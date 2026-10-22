NAME = HumanGL

CXX = c++
CC = cc

CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -O2 -g
CFLAGS = -Wall -Wextra -Werror -O2 -g
CPPFLAGS = -Iinclude -Iexternal/glad/include -MMD -MP

LDFLAGS =
LDLIBS = -lglfw -ldl -pthread -lm

SRC_DIR = src
OBJ_DIR = obj
GLAD_DIR = external/glad

SRCS = $(shell find $(SRC_DIR) -name '*.cpp' | sort)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
	$(OBJ_DIR)/glad.o

DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	@echo "$(NAME) build complete"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/glad.o: $(GLAD_DIR)/src/glad.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: all
	./$(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all run clean fclean re
