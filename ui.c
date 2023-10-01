#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

#include "ui.h"
#include "magi_ui.h"
#include "xterm_colors.h"

void render_fdline(int fd, int i, fd_set *s, int maxfds) {
    // unicode block elements to represent 0..15:
    // ' ', '▘', '▝', '▀', '▖', '▌', '▞', '▛', '▗', '▚', '▐', '▜', '▄', '▙', '▟', '█'
    const unsigned char blocks[16][3] = {
        { 0x00, 0x00, 0x00 },
        { 0xe2, 0x96, 0x98 },
        { 0xe2, 0x96, 0x9d },
        { 0xe2, 0x96, 0x80 },
        { 0xe2, 0x96, 0x96 },
        { 0xe2, 0x96, 0x8c },
        { 0xe2, 0x96, 0x9e },
        { 0xe2, 0x96, 0x9b },
        { 0xe2, 0x96, 0x97 },
        { 0xe2, 0x96, 0x9a },
        { 0xe2, 0x96, 0x90 },
        { 0xe2, 0x96, 0x9c },
        { 0xe2, 0x96, 0x84 },
        { 0xe2, 0x96, 0x99 },
        { 0xe2, 0x96, 0x9f },
        { 0xe2, 0x96, 0x88 },
    };

    // set color to red
    dprintf(fd, "\033[31m");
    for (int j = 0; j < 128; j += 4) {
        int box_idx = 0;
        for (int k = 0; k < 4; k++) {
            if (i + j + k >= maxfds) break;
            if (FD_ISSET(i + j + k, s)) {
                box_idx |= 1 << k;
            }
        }
        if (box_idx == 0) {
            dprintf(fd, " ");
        }
        else {
            write(fd, blocks[box_idx], 3);
        }
    }
    // reset color
    dprintf(fd, "\033[0m");
}

// Draw the three fd_sets using the unicode block elements
void render_fdset(int fd, session_t *s) {
    const unsigned char spacer[32];
    const unsigned char vertical_line[] = { 0xe2, 0x95, 0x91 };
    const unsigned char left_diag[] = { 0xe2, 0x95, 0xb1 };
    const unsigned char right_diag[] = { 0xe2, 0x95, 0xb2 };
    const unsigned char casper[] = { 0xe2, 0x95, 0x94, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0x20, 0x43, 0x41, 0x53, 0x50, 0x45, 0x52, 0x2d, 0x33, 0x20, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x97 };
    const unsigned char balthasar[] = { 0xe2, 0x95, 0x94, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0x20, 0x42, 0x41, 0x4c, 0x54, 0x48, 0x41, 0x53, 0x41, 0x52, 0x2d, 0x32, 0x20, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x97 };
    const unsigned char melchior[] = { 0xe2, 0x95, 0x94, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0x20, 0x4d, 0x45, 0x4c, 0x43, 0x48, 0x49, 0x4f, 0x52, 0x2d, 0x31, 0x20, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x97 };
    const unsigned char bottom[] = { 0xe2, 0x95, 0x9a, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x9d };
    const unsigned char bar[] = { 0xe2, 0x95, 0xa0, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0x90, 0xe2, 0x95, 0xa3 };
    memset((void *)spacer, ' ', sizeof(spacer));

    pthread_mutex_lock(&s->sensor_lock);

    // Balthasar
    write(fd, spacer, 24);
    write(fd, balthasar, sizeof(balthasar));
    dprintf(fd, "\n");
    for (int i = 0; i < s->maxfds; i += 128) {
        write(fd, spacer, 24);
        write(fd, vertical_line, sizeof(vertical_line));
        render_fdline(fd, i, &s->readfds, s->maxfds);
        write(fd, vertical_line, sizeof(vertical_line));
        dprintf(fd, "\n");
    }
    write(fd, spacer, 24);
    write(fd, bottom, sizeof(bottom));
    dprintf(fd, "\n");
    // Diagonal connectors
    write(fd, spacer, 31);
    write(fd, left_diag, sizeof(left_diag));
    write(fd, spacer, 18);
    write(fd, right_diag, sizeof(right_diag));
    dprintf(fd, "\n");
    write(fd, spacer, 30);
    write(fd, left_diag, sizeof(left_diag));
    write(fd, spacer, 20);
    write(fd, right_diag, sizeof(right_diag));
    dprintf(fd, "\n");

    // Casper and Melchior
    write(fd, spacer, 2);
    write(fd, casper, sizeof(casper));
    write(fd, spacer, 10);
    write(fd, melchior, sizeof(melchior));
    dprintf(fd, "\n");
    for (int i = 0; i < s->maxfds; i += 128) {
        write(fd, spacer, 2);
        write(fd, vertical_line, sizeof(vertical_line));
        render_fdline(fd, i, &s->writefds, s->maxfds);
        if (i == 512) {
            write(fd, bar, sizeof(bar));
        }
        else {
            write(fd, vertical_line, sizeof(vertical_line));
            write(fd, spacer, 10);
            write(fd, vertical_line, sizeof(vertical_line));
        }
        render_fdline(fd, i, &s->exceptfds, s->maxfds);
        write(fd, vertical_line, sizeof(vertical_line));
        dprintf(fd, "\n");
    }
    write(fd, spacer, 2);
    write(fd, bottom, sizeof(bottom));
    write(fd, spacer, 10);
    write(fd, bottom, sizeof(bottom));
    dprintf(fd, "\n");
    pthread_mutex_unlock(&s->sensor_lock);
}

void render_cell(int fd, ui_cell_t *cell) {
    if (cell->flags & UI_SKIP) return;
    if (cell->flags & UI_STYLE_RESET) {
        dprintf(fd, ANSI_RESET);
    }
    if (cell->flags & UI_STYLE_BOLD) {
        dprintf(fd, ANSI_BOLD);
    }
    if (cell->flags & UI_STYLE_DIM) {
        dprintf(fd, ANSI_DIM);
    }
    if (cell->flags & UI_STYLE_ITALIC) {
        dprintf(fd, ANSI_ITALIC);
    }
    if (cell->flags & UI_STYLE_UNDERLINE) {
        dprintf(fd, ANSI_UNDERLINED);
    }
    if (cell->flags & UI_STYLE_BLINK) {
        dprintf(fd, ANSI_BLINK);
    }
    if (cell->flags & UI_STYLE_REVERSE) {
        dprintf(fd, ANSI_REVERSE);
    }
    if (cell->flags & UI_STYLE_HIDDEN) {
        dprintf(fd, ANSI_HIDDEN);
    }
    if (cell->flags & UI_STYLE_STRIKETHROUGH) {
        dprintf(fd, ANSI_STRIKETHROUGH);
    }
    if (cell->flags & UI_STYLE_NONE) {
        dprintf(fd, ANSI_RESET);
    }
    if (cell->fg >= 0) {
        dprintf(fd, ANSI_FGCOLOR(%d), cell->fg);
    }
    if (cell->bg >= 0) {
        dprintf(fd, ANSI_BGCOLOR(%d), cell->bg);
    }
    write(fd, cell->bytes, cell->len);
    dprintf(fd, ANSI_RESET);
}

void render_surface(int fd, ui_surface_t *surface) {
    for (int i = 0; i < surface->width * surface->height; i++) {
        render_cell(fd, &surface->cells[i]);
    }
}

void update_cell(ui_cell_t *cell, ui_cell_t *other) {
    memcpy(cell->bytes, other->bytes, sizeof(cell->bytes));
    cell->len = other->len;
    cell->fg = other->fg == -1 ? cell->fg : other->fg;
    cell->bg = other->bg == -1 ? cell->bg : other->bg;
    if (other->flags & UI_STYLE_NONE)
        cell->flags |= other->flags;
    else
        cell->flags = other->flags;
}

void render_fdline_cells(ui_surface_t *surface, int row, int col, int i, fd_set *s, int maxfds) {
    // unicode block elements to represent 0..15:
    // ' ', '▘', '▝', '▀', '▖', '▌', '▞', '▛', '▗', '▚', '▐', '▜', '▄', '▙', '▟', '█'
    ui_cell_t blocks[16] = {
        { .bytes = { 0x20, 0x00, 0x00 }, .len = 1, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x98 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x9d }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x80 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x96 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x8c }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x9e }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x9b }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x97 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x9a }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x90 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x9c }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x84 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x99 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x9f }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
        { .bytes = { 0xe2, 0x96, 0x88 }, .len = 3, .fg = XTERM_COLOR_RED, .bg = -1, .flags = UI_STYLE_NONE },
    };

    int col_offset = 0;
    for (int j = 0; j < 128; j += 4) {
        int box_idx = 0;
        for (int k = 0; k < 4; k++) {
            if (i + j + k >= maxfds) break;
            if (FD_ISSET(i + j + k, s)) {
                box_idx |= 1 << k;
            }
        }
        if (box_idx == 0 && i + j > FD_SETSIZE) {
            // skip update
        }
        else {
            update_cell(CELL_AT(surface, row, col+col_offset), &blocks[box_idx]);
        }
        col_offset++;
    }
}

int printf_cells(ui_surface_t *surface, int row, int col,
                 ui_cell_flags_t style, int16_t fg, int16_t bg,
                 char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    // get some scratch space
    char scratch[1024];
    size_t maxlen = surface->width - col;
    if (maxlen > sizeof(scratch)) maxlen = sizeof(scratch);
    int n = vsnprintf(scratch, maxlen, fmt, args);
    va_end(args);
    if (n < 0) return n;
    if (n > maxlen) n = maxlen;
    for (int i = 0; i < n; i++) {
        ui_cell_t *cell = CELL_AT(surface, row, col+i);
        cell->bytes[0] = scratch[i];
        cell->len = 1;
        cell->flags = style;
        cell->fg = fg;
        cell->bg = bg;
    }
    return n;
}

void render_fdset_cells(ui_surface_t *surface, int row, int col, fd_set *s, int maxfds) {
    int row_offset = 0;
    for (int i = 0; i < maxfds; i += 128) {
        render_fdline_cells(surface, row + row_offset, col, i, s, maxfds);
        row_offset++;
    }
}

void blit_surface(ui_surface_t *dst, ui_surface_t *src, int dst_row, int dst_col) {
    for (int i = 0; i < src->height; i++) {
        for (int j = 0; j < src->width; j++) {
            update_cell(CELL_AT(dst, dst_row+i, dst_col+j), CELL_AT(src, i, j));
        }
    }
}

void debug_dumpflags(uint32_t flags) {
    if (flags & UI_SKIP) printf("UI_SKIP ");
    if (flags & UI_STYLE_RESET) printf("UI_STYLE_RESET ");
    if (flags & UI_STYLE_BOLD) printf("UI_STYLE_BOLD ");
    if (flags & UI_STYLE_DIM) printf("UI_STYLE_DIM ");
    if (flags & UI_STYLE_ITALIC) printf("UI_STYLE_ITALIC ");
    if (flags & UI_STYLE_UNDERLINE) printf("UI_STYLE_UNDERLINE ");
    if (flags & UI_STYLE_BLINK) printf("UI_STYLE_BLINK ");
    if (flags & UI_STYLE_REVERSE) printf("UI_STYLE_REVERSE ");
    if (flags & UI_STYLE_HIDDEN) printf("UI_STYLE_HIDDEN ");
    if (flags & UI_STYLE_STRIKETHROUGH) printf("UI_STYLE_STRIKETHROUGH ");
    if (flags & UI_STYLE_NONE) printf("UI_STYLE_NONE ");
    if (flags & UI_WIDE) printf("UI_WIDE ");
    printf("\n");
}

void debug_dumpcell(ui_surface_t *surface, int row, int col) {
    ui_cell_t *cell = CELL_AT(surface, row, col);
    printf("cell at (%d, %d):\n", row, col);
    printf("  bytes = {");
    for (int i = 0; i < cell->len; i++) {
        unsigned char c = cell->bytes[i];
        printf(" %02x", c);
        if (c >= 0x20 && c <= 0x7e) {
            printf(" '%c'", c);
        }
        printf(",");
    }
    printf(" }\n");
    printf("  bytes(str) = '%.*s'\n", cell->len, cell->bytes);
    int16_t fg = cell->fg;
    int16_t bg = cell->bg;
    printf("  fg = %s\n", fg == -1 ? "default" : xterm_color_names[fg]);
    printf("  bg = %s\n", bg == -1 ? "default" : xterm_color_names[bg]);
    printf("  flags = ");
    debug_dumpflags(cell->flags);
}

void render_fdsets_cells(ui_surface_t *surface, session_t *s) {
    pthread_mutex_lock(&s->sensor_lock);
    // Balthasar is at (3, 25)
    render_fdset_cells(surface, 3, 25, &s->readfds, s->maxfds);
    // Casper is at (14, 3)
    render_fdset_cells(surface, 14, 3, &s->writefds, s->maxfds);
    // Melchior is at (14, 47)
    render_fdset_cells(surface, 14, 47, &s->exceptfds, s->maxfds);
    // Number of fds is at (5, 14)
    printf_cells(surface, 5, 14, UI_STYLE_BOLD, XTERM_COLOR_DARKORANGE3_1, -1, "%-4d", s->nfds+1);
    // Add the infobox at (10, 70)
    printf("Before blit:\n");
    for (int c = 0; c < magi_ui_red_box.width; c++)
        debug_dumpcell(surface, 11, 70+c);
    blit_surface(surface, &magi_ui_red_box, 10, 70);
    printf("After blit:\n");
    for (int c = 0; c < magi_ui_red_box.width; c++)
        debug_dumpcell(surface, 11, 70+c);
    pthread_mutex_unlock(&s->sensor_lock);
}
