#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

#define WIDTH 50
#define HEIGHT 35

int main() {
    /* get the connection */
    int screenNum;
    xcb_connection_t *connection = xcb_connect(NULL, &screenNum);
    if (!connection) {
        fprintf(stderr, "ERROR: can't connect to an X server\n");
        return -1;
    }

    /* get the current screen */
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));

    /* we want the screen at index screenNum of the iterator */
    for (int i = 0; i < screenNum; ++i) {
        xcb_screen_next(&iter);
    }

    xcb_screen_t *screen = iter.data;
    if (!screen) {
        fprintf(stderr, "ERROR: can't get the current screen\n");
        xcb_disconnect(connection);
        return -1;
    }

    /* create the window */
    xcb_window_t window = xcb_generate_id(connection);
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2];
    values[0] = screen->white_pixel;
    values[1] = XCB_EVENT_MASK_KEY_RELEASE |
                XCB_EVENT_MASK_BUTTON_PRESS |
                XCB_EVENT_MASK_EXPOSURE |
                XCB_EVENT_MASK_POINTER_MOTION;

    xcb_void_cookie_t windowCookie = xcb_create_window(connection, screen->root_depth, window, screen->root, 10, 30, WIDTH, HEIGHT, 213,
                                                               XCB_WINDOW_CLASS_INPUT_OUTPUT,
                                                               screen->root_visual, mask, values);

    // xcb_ewmh_connection_t *ewmh = NULL;  // Ewmh Connection
    // if (!(ewmh = calloc(1, sizeof(xcb_ewmh_connection_t)))) {
    //     printf("Fail to calloc memory for EWMH\n");
    // }

    // xcb_intern_atom_cookie_t *cookie = xcb_ewmh_init_atoms(connection, ewmh);
    // if (!xcb_ewmh_init_atoms_replies(ewmh, cookie, (void *)0)) {
    //     fprintf(stderr, "%s\n", "xcb_ewmh_init_atoms_replies:faild.");
    //     exit(1);
    // }

    xcb_map_window(connection, window);
    xcb_flush(connection);

    // int curws = 1;
    // xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, ewmh->_NET_WM_DESKTOP, XCB_ATOM_CARDINAL, 32, 1, &curws);

    sleep(2);
}
