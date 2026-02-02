CXXFLAGS      = -Wall -Wextra -Werror -Wshadow -Iincludes -std=c++98 # -g -g3 -ggdb3 -fsanitize=address
SRC           = $(wildcard *.cpp ./src/*.cpp)
OBJ           = $(SRC:%.cpp=$(OBJDIR)/%.o)
OBJDIR        = builds
NAME          = WebServ
CXX           = c++

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o$@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) $(LIB_DIR)

re: fclean all

.SECONDARY: $(OBJ)
.PHONY: all clean fclean re
