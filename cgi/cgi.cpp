#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int    main()
{
	char    buf[512] = {0};
	int     count;

	count = read(0, buf, 511);
	if (count != -1)
		write(1, buf, count);
	else
		write(1, "fail\0", 5);
	return 0;
}
