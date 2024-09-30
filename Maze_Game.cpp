#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h> 

int maze[50][50];

//构建maze数组与迷宫各组成部分的映射
void print_based_on_int(int x){
	switch (x){
	case 0:
		printf("█");//墙壁
		break;
	case 1:
		printf(" ");//道路
		break;
	case 2:
		printf("S");//起点
		break;
	case 3:
		printf("E");//终点
		break;
	case 4:
		printf("▲");//玩家所在位置
		break;
	case 5:
		printf("×");//指路标记
		break;
	default:
		break;
	}
}

//随机生成介于两个整数之间的整数的函数
int random_int(int min,int max){
	return min+rand()%(max-min+1);
}

//判断某点是否在迷宫内
int in_bound(int x,int y,int length,int width){
	return (x>0 && x<length-1 && y>0  &&y<width-1);
}

//生成随机的方向数组
int** generate_random_directions(){
	//生成0、1、2、3的随机排列
	int i=0;
	int j[] = {0,1,2,3};
	for(int i=3;i>0;i--){
		int k = random_int(0, i);
        int temp = j[k];
        j[k] = j[i];
        j[i] = temp;
    }

	int temp_directions[4][2] = {{-1, 0},{0, 1},{1, 0},{0, -1}};
	int **directions = (int **)malloc(4*sizeof(int *));
	for(int i=0;i<4;i++){
		directions[i] = (int *)malloc(2*sizeof(int));
	}
	for(int i=0;i<4;i++){
		directions[i][0] = temp_directions[j[i]][0];
		directions[i][1] = temp_directions[j[i]][1];
	}
	return directions;
}

//用DFS算法挖出迷宫的通道
void DFS_generate_passway(int start_x,int start_y,int length,int width){

	int **directions = generate_random_directions();

	int x=start_x,y=start_y;
	maze[x][y] = 1;

    for(int i=0;i<4;i++){
		int mx = x+directions[i][0];
		int my = y+directions[i][1];
		int nx = x+directions[i][0]*2;
		int ny = y+directions[i][1]*2;
		
		if(in_bound(nx,ny,length,width)==1 && maze[nx][ny]==0){
			maze[mx][my] = 1;
			DFS_generate_passway(nx,ny,length,width);
		}
	}
}

//用prim算法挖出迷宫的通道
void prim_generate_passway(int start_x,int start_y,int length,int width){
	maze[start_x][start_y] = 1;
	
	//记录已探明的可挖掘区域
	int n = (length*width)/2;
	int *frontier_x = (int *)malloc(n * sizeof(int));
	int *frontier_y = (int *)malloc(n * sizeof(int));
	frontier_x[0] = start_x;
	frontier_y[0] = start_y;
	int frontier_num = 1;

	while(frontier_num > 0){
		int index = random_int(0,frontier_num-1);
		int current_x = frontier_x[index];
		int current_y = frontier_y[index];

		//寻找未挖的临近点
		int near_directions[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
		int neighbors[4][2];
		int neighbors_num = 0;

		for(int i=0;i<4;i++){
			int nx = current_x + near_directions[i][0]*2;
			int ny = current_y + near_directions[i][1]*2;
			if (in_bound(nx,ny,length,width) && maze[nx][ny] == 0){
				neighbors[neighbors_num][0] = nx;
				neighbors[neighbors_num][1] = ny;
                neighbors_num++;
			}
		}

        if(neighbors_num > 0){
			//随机选取临近点
			int random_neighbor_num = random_int(0,neighbors_num-1);
			int nx = neighbors[random_neighbor_num][0] , mx = (current_x + nx) / 2;
			int ny = neighbors[random_neighbor_num][1] , my = (current_y + ny) / 2;
			
			maze[mx][my] = 1;
			maze[nx][ny] = 1;
			
			//将新探明的点存入数组
			frontier_x[frontier_num] = nx;
			frontier_y[frontier_num] = ny;
			frontier_num++;
		}
		else{
			//移去无法再挖的点
			frontier_x[index] = frontier_x[frontier_num - 1];
			frontier_y[index] = frontier_y[frontier_num - 1];
			frontier_num--;
		}		
	}
}

//输入长度或宽度
void input_length_or_width(int *x){
	char *input = (char *)malloc(50 * sizeof(char));
	scanf("%s",input);
	if(isdigit(*input)){
		*x = atoi(input);
		if(*x<=0||*x>50)
			printf("无效输入！\n");
	}
	else
		printf("无效输入！\n");
	free(input);
}

//随机生成具有给定长度、宽度的迷宫
void generate_maze(int length,int width,int *end_x,int *end_y,int *player_x,int *player_y){

	//初始化迷宫，全部设为墙壁
	for(int i=0;i<length; i++){
		for (int j=0;j<width;j++){
			maze[i][j] = 0;
		}
	}

	//从起点挖掘通道
	int start_x = 1,start_y = 1;
	while(1){
		printf("\n请选择迷宫类型\n按D生成第一类迷宫：路线更曲折\n按G生成第二类迷宫：分岔口更多\n（按其他键是不会进入下一步的 :) ）\n");//选择生成迷宫的种类
		char choice = getch();
		if(choice=='d'||choice=='D'){
			DFS_generate_passway(start_x,start_y,length,width);
			break;
		}
		if(choice=='g'||choice=='G'){
			prim_generate_passway(start_x,start_y,length,width);
			break;
		}
	}

	//确定终点位置，终点设置在挖出的通道中，且不与起点重合
	while(1){
		*end_x = random_int(length/2,length-2);
		*end_y = random_int(width/2,width-2);
		if(maze[*end_x][*end_y]==1){
			maze[*end_x][*end_y] = 3;
			break;
		}
	}

	//设定玩家起始位置
	maze[1][1] = 4;
	*player_x = 1;
	*player_y = 1;
}

//打印迷宫
void print_maze(int length,int width){
	printf("迷宫布局：\n█为墙壁  空格为可走通道\n▲为玩家所在位置  S为起点  E为终点\n\n");
	printf("操作方法：\n按W、S、A、D键分别可向上、下、左、右移动一格\n\n");
	printf("按H键开启自动寻路功能,屏幕上×为指示下一步的指路标记\n按N键关闭自动寻路功能\n\n");
	for(int i=0;i<length;i++){
		for(int j=0;j<width;j++){
			print_based_on_int(maze[i][j]);
		}
		printf("\n");
	}
}

//覆盖打印
void reprint_maze(int length,int width){
	//   清屏不闪烁的废稿\
	     printf("\033[%d;%dH",0,0);\
	     fflush(stdout);
	
	system("cls");
	print_maze(length,width);
}

//玩家试图移动至墙壁、边界时给出提示
void reminder(int x){
	if(x==0){
		printf("你不能移动至墙壁或离开迷宫边界！");
		fflush(stdout);
	}
	Sleep(900);
	printf("\033[2K"); //清除当前行
    printf("\033[0G"); //将光标移到行首
    fflush(stdout);
}

//玩家移动
void move(int* player_x,int* player_y,int length,int width,int dx,int dy){
	int new_x = *player_x+dx;
	int new_y = *player_y+dy;
	
	switch (maze[new_x][new_y]) {
	case 0:
		reminder(0);
		break;
	case 1:
	case 2:
	case 3:
		maze[new_x][new_y] = 4;
		if(*player_x==1 && *player_y==1)
			maze[*player_x][*player_y] = 2;
		else
			maze[*player_x][*player_y] = 1;
		*player_x = new_x;
		*player_y = new_y;
		break;
	}
}

//自动寻路
void find_way(int *end_x,int *end_y,int *player_x,int *player_y,int *where_to_go_x,int *where_to_go_y,int length,int width){
	int last_step_x[50][50],last_step_y[50][50];//记录找路走到此处时上一步在何处，用来确定路径
	int queue_x[2500],queue_y[2500];//按序存放找路路径
	int visited[50][50] = {0};//表示通道是否被访问过
	
	int dx[] = {-1, 1,  0, 0};
	int dy[] = { 0, 0, -1, 1};
	int front = 1;//被探索过的点的个数
	int check = 0;//周边的点也被探索过的点的个数
	int found = 0;//表示是否找到路
	
	queue_x[0] = *player_x;
	queue_y[0] = *player_y;
	visited[*player_x][*player_y] = 1;
	
	//为通道匹配对应的last_step，使得探索到的每个点沿着last_step链均可到达出发点
	while (check < front && !found){
		int current_x = queue_x[check];
		int current_y = queue_y[check];
		check++;
		
		for (int i=0;i<4;i++){
			int nx = current_x + dx[i];
			int ny = current_y + dy[i];

			if(in_bound(nx,ny,length,width) && maze[nx][ny]!=0 && !visited[nx][ny]){
				queue_x[front] = nx;
				queue_y[front] = ny;
				front++;
				visited[nx][ny] = 1;

				last_step_x[nx][ny] = current_x;
				last_step_y[nx][ny] = current_y;

				if(nx==*end_x && ny==*end_y){
					found = 1;
					break;
				}
			}
		}
	}

	//确定玩家位置到终点的路径
	int path_length = 0;
	int path_x[2500],path_y[2500];
	int cx = *end_x, cy = *end_y;
	while(1){
		path_x[path_length] = cx;
		path_y[path_length] = cy;
		path_length++;

		int temp_x = last_step_x[cx][cy];
		int temp_y = last_step_y[cx][cy];
		cx = temp_x;
		cy = temp_y;
		if(cx==*player_x && cy==*player_y)
			break;
	}

	//标记指路标示
	*where_to_go_x = path_x[path_length - 1];
	*where_to_go_y = path_y[path_length - 1];
	if(*where_to_go_x!=*end_x || *where_to_go_y!=*end_y)    //走一步就能到终点时不必明示指路标
		maze[path_x[path_length - 1]][path_y[path_length - 1]] = 5;
}

//游玩游戏
void play_game(){
	srand((unsigned)time(NULL));
	int length = 0,width = 0,need_help = 0;
	printf("---------Maze Game---------\n欢迎游玩本迷宫游戏！\n");
	while(1){
		printf("请输入迷宫长度（不得大于50格）：");
		input_length_or_width(&length);
		if(length>0 && length<=50)
			break;
	}

	while(1){
		printf("请输入迷宫宽度（不得大于50格）：");
		input_length_or_width(&width);
		if(width>0 && width<=50)
			break;
	}

	int end_x,end_y;
	int player_x,player_y;
	int where_to_go_x = -1,where_to_go_y = -1;

	generate_maze(length,width,&end_x,&end_y,&player_x,&player_y);
	system("cls");
	print_maze(length,width);

	int dx[] = {-1, 1, 0, 0};
	int dy[] = { 0, 0,-1, 1};//方向数组，分别表示向上、向下、向左、向右的位移，与w、s、a、d一一对应
	char direction_keys_1[] = {'w','s','a','d'};
	char direction_keys_2[] = {'W','S','A','D'};
	
	//接收玩家的操作
	while(1){
		//清去上一次指路的痕迹（如果有）
		if(where_to_go_x != -1 && where_to_go_y != -1)
			maze[where_to_go_x][where_to_go_y] = 1;
		maze[1][1] = 2;
		maze[end_x][end_y] = 3;
		maze[player_x][player_y] = 4;
		
		int need_reprint = 0;
		char input = getch();
		for (int i=0;i<4;i++){
			if(input==direction_keys_1[i]){
				move(&player_x,&player_y,length,width,dx[i],dy[i]);
				need_reprint = 1;
				break;
            }
			if(input==direction_keys_2[i]){
				move(&player_x,&player_y,length,width,dx[i],dy[i]);
				need_reprint = 1;
				break;
            }
        }
		if(input=='h'||input=='H'){
			need_help = 1;
			need_reprint = 1;
		}
		if(input=='n'||input=='N'){
			need_help = 0;
			maze[where_to_go_x][where_to_go_y] = 1;
			maze[end_x][end_y] = 3;
			need_reprint = 1;
		}

		//判定是否到达终点
		if(end_x==player_x && end_y==player_y){
			reprint_maze(length,width);
			printf("恭喜你过关！\n");
			break;
		}

		//判定是否需要引路
		if(need_help)
			find_way(&end_x,&end_y,&player_x,&player_y,&where_to_go_x,&where_to_go_y,length,width);

		//判定是否需要覆盖打印
		if(need_reprint)
			reprint_maze(length,width);
	}
}

//在主函数实现重新游玩功能
int main(){
	char want_to_play;
	do{
		system("cls");
		play_game();
		printf("\n是否要重新游玩本游戏？\n按Y重新开始  按其他任意键结束游玩\n");
		want_to_play = getch();
	}while(want_to_play=='y'||want_to_play=='Y');
	return 0;
}