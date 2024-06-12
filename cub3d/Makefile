NAME				=	cub3D

NAME_B				=	cub3D_bonus

# FLAGS				=	-Wall -Werror -Wextra

COMPILE				=	cc
#-fsanitize=address -g

MLX_LINUX_FLAGS		=	-ldl -pthread -lm
MLX_MAC_FLAGS		=	-framework Cocoa -framework OpenGL -framework IOKit
MLXINCLUDE			=	MLX42/build/libmlx42.a -Iinclude -lglfw

COLOR_RESET			=	\033[0m
COLOR_CYAN			=	\033[36m
COLOR_GREEN			=	\033[32m
COLOR_RED			=	\033[31m

INCLUDE				=	libft/
REPO_URL			=	https://github.com/codam-coding-college/MLX42.git
LOCAL_DIR			=	MLX42

# List of all source files
SRCS				=	dda_algo/main.c # Add other source files as needed

OBJS				=	$(SRCS:.c=.o)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	MLX_FLAGS := $(MLX_LINUX_FLAGS)
else ifeq ($(UNAME_S), Darwin)
	MLX_FLAGS := $(MLX_MAC_FLAGS)
else
	$(error Unsupported operating system: $(UNAME_S))
endif

all: $(NAME)

$(NAME): clone $(OBJS)
					@$(COMPILE) $(FLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(MLXINCLUDE) $(MLX_FLAGS)
					@echo "$(COLOR_CYAN)[READY] $(NAME)$(COLOR_RESET)"

%.o: %.c
					@$(COMPILE) $(FLAGS) -c $< -o $@
					@echo "$(COLOR_GREEN)[OK] $@$(COLOR_RESET)"

clone:
					@if [ -d $(LOCAL_DIR) ]; then \
						echo "$(COLOR_CYAN)Repository already cloned$(COLOR_RESET)"; \
					else \
						echo "$(COLOR_GREEN)Clone and build mlx$(COLOR_RESET)"; \
						git clone $(REPO_URL) $(LOCAL_DIR) && \
						cd $(LOCAL_DIR) && \
						cmake -B build && \
						cd build && \
						make; \
					fi

remove:				fclean
					@if [ -d $(LOCAL_DIR) ]; then \
						rm -rf $(LOCAL_DIR); \
					else \
						echo "$(COLOR_RED)$(LOCAL_DIR) does not exist$(COLOR_RESET)"; \
					fi

clean:
					@echo "$(COLOR_RED)Cleanup cub3d.$(COLOR_RESET)"
					@rm -f $(OBJS)
					@echo "$(COLOR_GREEN)Cleanup completed.$(COLOR_RESET)"

fclean:				clean
					@echo "$(COLOR_RED)Full-Clean cub3d.$(COLOR_RESET)"
					@rm -f $(NAME)
					@echo "$(COLOR_GREEN)Full-Clean completed.$(COLOR_RESET)"

re:					fclean all

.PHONY:				all clean fclean re clone bonus
