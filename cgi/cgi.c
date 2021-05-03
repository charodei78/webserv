#include <fcntl.h>
#include <unistd.h>

int    main()
{
	char    buf[512];
	int     count;

	count = read(0, buf, 512);
	if (count != -1)
		write(1, buf, count);
	else
		write(1, "fail\0", 5);
	return 0;
}
