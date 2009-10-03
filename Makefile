CC = gcc

CFLAGS = -g -Wall -O2

target = ime2anthy

$(target):
	$(CC) $(target).c $(CFLAGS) `pkg-config --cflags --libs glib-2.0` -o $(target)

clean:
	rm -f $(target) core *~
