NAME	= computor
SRCS	= comptor.cpp

OBJS	= $(SRCS:.cpp=.o)
FLAGS	= -Wextra -Wall -Werror -std=c++11
CC		= g++
INCS	= .

# Règle pour générer les fichiers .o à partir des .cpp
.cpp.o:
	@$(CC) $(FLAGS) -c $< -o $@ -I$(INCS)

# Règle principale pour générer l'exécutable
all: $(NAME)

$(NAME): $(OBJS)
	@clear
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "Build successful!"

# Nettoyage des fichiers objets
clean:
	@echo "delete : $(OBJS)"
	@rm -rf $(OBJS)

# Nettoyage complet : objets et exécutable
fclean: clean
	@echo "delete : $(NAME)"
	@rm -rf $(NAME)

# Rebuild complet (nettoie tout puis reconstruit)
re: fclean all

.PHONY: all clean fclean re