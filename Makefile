NAME = webserv

SRCS = main.cpp Request.cpp Cgi.cpp Response.cpp Sockets.cpp Config.cpp Server.cpp Location.cpp Utils.cpp

OBJS = $(SRCS:.cpp=.o)

CC = clang++

CFLAGS = -Wall -Wextra -Werror -g -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -g

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	/bin/rm -f *.o

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
