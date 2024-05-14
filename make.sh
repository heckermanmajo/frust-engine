gcc -pedantic -Wall -Wextra -fsanitize=address  main.c libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11 \
&& ./a.out