/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TTY_FLIP_H
#define _LINUX_TTY_FLIP_H

#include </home/runner/EulerBot/linux/tty_buffer.h>
#include </home/runner/EulerBot/linux/tty_port.h>

struct tty_ldisc;

int tty_buffer_set_limit(struct tty_port *port, int limit);
unsigned int tty_buffer_space_avail(struct tty_port *port);
int tty_buffer_request_room(struct tty_port *port, size_t size);
int tty_insert_flip_string_flags(struct tty_port *port,
		const unsigned char *chars, const char *flags, size_t size);
int tty_insert_flip_string_fixed_flag(struct tty_port *port,
		const unsigned char *chars, char flag, size_t size);
int tty_prepare_flip_string(struct tty_port *port, unsigned char **chars,
		size_t size);
void tty_flip_buffer_push(struct tty_port *port);
int __tty_insert_flip_char(struct tty_port *port, unsigned char ch, char flag);

static inline int tty_insert_flip_char(struct tty_port *port,
					unsigned char ch, char flag)
{
	struct tty_buffer *tb = port->buf.tail;
	int change;

	change = !tb->flags && (flag != TTY_NORMAL);
	if (!change && tb->used < tb->size) {
		if (tb->flags)
			*flag_buf_ptr(tb, tb->used) = flag;
		*char_buf_ptr(tb, tb->used++) = ch;
		return 1;
	}
	return __tty_insert_flip_char(port, ch, flag);
}

static inline int tty_insert_flip_string(struct tty_port *port,
		const unsigned char *chars, size_t size)
{
	return tty_insert_flip_string_fixed_flag(port, chars, TTY_NORMAL, size);
}

int tty_ldisc_receive_buf(struct tty_ldisc *ld, const unsigned char *p,
		const char *f, int count);

void tty_buffer_lock_exclusive(struct tty_port *port);
void tty_buffer_unlock_exclusive(struct tty_port *port);

#endif /* _LINUX_TTY_FLIP_H */
