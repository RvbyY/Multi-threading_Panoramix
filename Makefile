## ----------------------------------------------------------------------------------- ##
##                                                                                     ##
## EPITECH PROJECT - Fri, May, 2026                                                    ##
## Title           - G-CCP-400-STG-4-1-panoramix-16                                    ##
## Description     -                                                                   ##
##     Makefile                                                                        ##
##                                                                                     ##
## ----------------------------------------------------------------------------------- ##
##                                                                                     ##
##         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░            ##
##         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒            ##
##         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓            ##
##         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █            ##
##         █        █  ███████        ████  ████        ██      ██  ████  █            ##
##                                                                                     ##
## ----------------------------------------------------------------------------------- ##

NAME = panoramix

CXX = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -I./include -g

# Source files
SRCS = 	src/main.c \
		src/villager.c \
		src/druid.c \

OBJS = $(SRCS:.cpp=.o)

# Colors for output
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m # No Color

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(NC)"
	@$(CXX) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Build complete!$(NC)"

%.o: %.cpp
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@find src -name '*.o' -delete
	@rm -f $(NAME)
	@echo "$(RED)Clean complete!$(NC)"

fclean: clean
	@echo "$(RED)Removing executable...$(NC)"
	@rm -f $(NAME)
	@echo "$(RED)Full clean complete!$(NC)"

re: fclean all

.PHONY: all clean fclean re