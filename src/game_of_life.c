#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#define BORDER_CHAR		'*'
#define LIVE_CELL			'@'
#define DEAD_CELL			' '

typedef struct game_of_life
{
    char map[25][81];
    int speed;
    int stop;
} t_life;

int		read_from_file(t_life *game, char *file_name);
void	ft_print_map(t_life *game);
int		ft_next_step(t_life *game );
int		check_cell(t_life *game, int x, int y);
void	map_copy(t_life *game, char tmp_map[][81]);
int		map_cmp(t_life *game, char tmp_map[][81]);
void	*ft_change_speed(void *game);


int main(void){
    t_life		game;
    pthread_t	pid;
    
    game.speed = 1000000;
    game.stop = 0;
    if (read_from_file(&game, "game_of_life_1.txt") != 0)
    {
        printf("n/a");
        return (1);
    }
    pthread_create(&pid, NULL, ft_change_speed, &game);
    

    do
    {

        ft_print_map(&game);
        usleep(game.speed);
    }
    while (ft_next_step(&game)  == 1 && game.stop == 0);
    pthread_join(pid, NULL);
    
    return (0);
}

int		check_map_line(char *str, int i)
{
    if (i == 0 || i == 24){
        for (int i = 0; i < 80; i++)
            if (str[i] != BORDER_CHAR)
                return (1);
    } else {
        if (str[0] != BORDER_CHAR || str[79] != BORDER_CHAR)
            return (1);
        for (int i = 1; i < 79; i++)
            if (str[i] != LIVE_CELL && str[i] != DEAD_CELL)
                return (1);
    }
    return (0);
}

int		read_from_file(t_life *game, char *file_name){
    FILE	*file;
    
    file = fopen(file_name, "r");
    if (file == NULL)
        return (1);
    for (int i = 0; i < 25; i++)
    {
        if(fread(game->map[i], 1, 81, file) != 81)
            return (1);
        game->map[i][80] = '\0';
        if (check_map_line(game->map[i], i) == 1)
            return (1);
    }
    fclose(file);
    return (0);
}

void	ft_print_map(t_life *game){
    for (int i = 0; i < 25; i++)
        printf("%s\n", game->map[i]);
    return ;
}

int		ft_next_step(t_life *game ){
    char	tmp_map[25][81];
    int		neighbors;
    
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            neighbors = check_cell(game, i, j);
            if (game->map[i][j] == LIVE_CELL && (neighbors < 2 || neighbors > 3))
                tmp_map[i][j] = DEAD_CELL;
            else if (game->map[i][j] == DEAD_CELL && neighbors == 3)
                tmp_map[i][j] = LIVE_CELL;
            else
                tmp_map[i][j] = game->map[i][j];
        }
    }
    if (map_cmp(game, tmp_map) == 1)
        return (0);
    map_copy(game, tmp_map);
    return (1);
}

int		check_cell(t_life *game, int i, int j){
    int	n;
    
    if (i > 23 || i < 1 || i < 1 || j > 78)
        return (0);
    n = 0;
    if(game->map[i - 1][j] == LIVE_CELL)
        n++;
    if(game->map[i + 1][j] == LIVE_CELL)
        n++;
    if(game->map[i][j - 1] == LIVE_CELL)
        n++;
    if(game->map[i][j + 1] == LIVE_CELL)
        n++;
    if(game->map[i + 1][j - 1] == LIVE_CELL)
        n++;
    if(game->map[i + 1][j + 1] == LIVE_CELL)
        n++;
    if(game->map[i - 1][j - 1] == LIVE_CELL)
        n++;
    if(game->map[i - 1][j + 1] == LIVE_CELL)
        n++;
    return (n);
}

void	map_copy(t_life *game, char tmp_map[][81]){
    
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            game->map[i][j] = tmp_map[i][j];
        }
    }
    return;
}

int		map_cmp(t_life *game, char tmp_map[][81]){
    
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            if (game->map[i][j] != tmp_map[i][j])
                return (0);
        }
    }
    return (1);
}

void	*ft_change_speed(void *arg){
    int c;
    t_life *game;
    
    game = (t_life *)arg;
    c = getc(stdin);
    while (c != 'q'){

        if (c == 'a' && game->speed <= 3000000)
            game->speed += 100000;
        else if (c == 'z' && game->speed > 200000)
            game->speed -= 100000;
        c = getc(stdin);
    }
    game->stop = 1;
    return (NULL);
}
