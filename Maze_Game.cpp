#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h> 

int maze[50][50];

//����maze�������Թ�����ɲ��ֵ�ӳ��
void print_based_on_int(int x){
	switch (x){
	case 0:
		printf("��");//ǽ��
		break;
	case 1:
		printf(" ");//��·
		break;
	case 2:
		printf("S");//���
		break;
	case 3:
		printf("E");//�յ�
		break;
	case 4:
		printf("��");//�������λ��
		break;
	case 5:
		printf("��");//ָ·���
		break;
	default:
		break;
	}
}

//������ɽ�����������֮��������ĺ���
int random_int(int min,int max){
	return min+rand()%(max-min+1);
}

//�ж�ĳ���Ƿ����Թ���
int in_bound(int x,int y,int length,int width){
	return (x>0 && x<length-1 && y>0  &&y<width-1);
}

//��������ķ�������
int** generate_random_directions(){
	//����0��1��2��3���������
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

//��DFS�㷨�ڳ��Թ���ͨ��
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

//��prim�㷨�ڳ��Թ���ͨ��
void prim_generate_passway(int start_x,int start_y,int length,int width){
	maze[start_x][start_y] = 1;
	
	//��¼��̽���Ŀ��ھ�����
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

		//Ѱ��δ�ڵ��ٽ���
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
			//���ѡȡ�ٽ���
			int random_neighbor_num = random_int(0,neighbors_num-1);
			int nx = neighbors[random_neighbor_num][0] , mx = (current_x + nx) / 2;
			int ny = neighbors[random_neighbor_num][1] , my = (current_y + ny) / 2;
			
			maze[mx][my] = 1;
			maze[nx][ny] = 1;
			
			//����̽���ĵ��������
			frontier_x[frontier_num] = nx;
			frontier_y[frontier_num] = ny;
			frontier_num++;
		}
		else{
			//��ȥ�޷����ڵĵ�
			frontier_x[index] = frontier_x[frontier_num - 1];
			frontier_y[index] = frontier_y[frontier_num - 1];
			frontier_num--;
		}		
	}
}

//���볤�Ȼ���
void input_length_or_width(int *x){
	char *input = (char *)malloc(50 * sizeof(char));
	scanf("%s",input);
	if(isdigit(*input)){
		*x = atoi(input);
		if(*x<=0||*x>50)
			printf("��Ч���룡\n");
	}
	else
		printf("��Ч���룡\n");
	free(input);
}

//������ɾ��и������ȡ���ȵ��Թ�
void generate_maze(int length,int width,int *end_x,int *end_y,int *player_x,int *player_y){

	//��ʼ���Թ���ȫ����Ϊǽ��
	for(int i=0;i<length; i++){
		for (int j=0;j<width;j++){
			maze[i][j] = 0;
		}
	}

	//������ھ�ͨ��
	int start_x = 1,start_y = 1;
	while(1){
		printf("\n��ѡ���Թ�����\n��D���ɵ�һ���Թ���·�߸�����\n��G���ɵڶ����Թ����ֲ�ڸ���\n�����������ǲ��������һ���� :) ��\n");//ѡ�������Թ�������
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

	//ȷ���յ�λ�ã��յ��������ڳ���ͨ���У��Ҳ�������غ�
	while(1){
		*end_x = random_int(length/2,length-2);
		*end_y = random_int(width/2,width-2);
		if(maze[*end_x][*end_y]==1){
			maze[*end_x][*end_y] = 3;
			break;
		}
	}

	//�趨�����ʼλ��
	maze[1][1] = 4;
	*player_x = 1;
	*player_y = 1;
}

//��ӡ�Թ�
void print_maze(int length,int width){
	printf("�Թ����֣�\n��Ϊǽ��  �ո�Ϊ����ͨ��\n��Ϊ�������λ��  SΪ���  EΪ�յ�\n\n");
	printf("����������\n��W��S��A��D���ֱ�����ϡ��¡������ƶ�һ��\n\n");
	printf("��H�������Զ�Ѱ·����,��Ļ�ϡ�Ϊָʾ��һ����ָ·���\n��N���ر��Զ�Ѱ·����\n\n");
	for(int i=0;i<length;i++){
		for(int j=0;j<width;j++){
			print_based_on_int(maze[i][j]);
		}
		printf("\n");
	}
}

//���Ǵ�ӡ
void reprint_maze(int length,int width){
	//   ��������˸�ķϸ�\
	     printf("\033[%d;%dH",0,0);\
	     fflush(stdout);
	
	system("cls");
	print_maze(length,width);
}

//�����ͼ�ƶ���ǽ�ڡ��߽�ʱ������ʾ
void reminder(int x){
	if(x==0){
		printf("�㲻���ƶ���ǽ�ڻ��뿪�Թ��߽磡");
		fflush(stdout);
	}
	Sleep(900);
	printf("\033[2K"); //�����ǰ��
    printf("\033[0G"); //������Ƶ�����
    fflush(stdout);
}

//����ƶ�
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

//�Զ�Ѱ·
void find_way(int *end_x,int *end_y,int *player_x,int *player_y,int *where_to_go_x,int *where_to_go_y,int length,int width){
	int last_step_x[50][50],last_step_y[50][50];//��¼��·�ߵ��˴�ʱ��һ���ںδ�������ȷ��·��
	int queue_x[2500],queue_y[2500];//��������··��
	int visited[50][50] = {0};//��ʾͨ���Ƿ񱻷��ʹ�
	
	int dx[] = {-1, 1,  0, 0};
	int dy[] = { 0, 0, -1, 1};
	int front = 1;//��̽�����ĵ�ĸ���
	int check = 0;//�ܱߵĵ�Ҳ��̽�����ĵ�ĸ���
	int found = 0;//��ʾ�Ƿ��ҵ�·
	
	queue_x[0] = *player_x;
	queue_y[0] = *player_y;
	visited[*player_x][*player_y] = 1;
	
	//Ϊͨ��ƥ���Ӧ��last_step��ʹ��̽������ÿ��������last_step�����ɵ��������
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

	//ȷ�����λ�õ��յ��·��
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

	//���ָ·��ʾ
	*where_to_go_x = path_x[path_length - 1];
	*where_to_go_y = path_y[path_length - 1];
	if(*where_to_go_x!=*end_x || *where_to_go_y!=*end_y)    //��һ�����ܵ��յ�ʱ������ʾָ·��
		maze[path_x[path_length - 1]][path_y[path_length - 1]] = 5;
}

//������Ϸ
void play_game(){
	srand((unsigned)time(NULL));
	int length = 0,width = 0,need_help = 0;
	printf("---------Maze Game---------\n��ӭ���汾�Թ���Ϸ��\n");
	while(1){
		printf("�������Թ����ȣ����ô���50�񣩣�");
		input_length_or_width(&length);
		if(length>0 && length<=50)
			break;
	}

	while(1){
		printf("�������Թ���ȣ����ô���50�񣩣�");
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
	int dy[] = { 0, 0,-1, 1};//�������飬�ֱ��ʾ���ϡ����¡��������ҵ�λ�ƣ���w��s��a��dһһ��Ӧ
	char direction_keys_1[] = {'w','s','a','d'};
	char direction_keys_2[] = {'W','S','A','D'};
	
	//������ҵĲ���
	while(1){
		//��ȥ��һ��ָ·�ĺۼ�������У�
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

		//�ж��Ƿ񵽴��յ�
		if(end_x==player_x && end_y==player_y){
			reprint_maze(length,width);
			printf("��ϲ����أ�\n");
			break;
		}

		//�ж��Ƿ���Ҫ��·
		if(need_help)
			find_way(&end_x,&end_y,&player_x,&player_y,&where_to_go_x,&where_to_go_y,length,width);

		//�ж��Ƿ���Ҫ���Ǵ�ӡ
		if(need_reprint)
			reprint_maze(length,width);
	}
}

//��������ʵ���������湦��
int main(){
	char want_to_play;
	do{
		system("cls");
		play_game();
		printf("\n�Ƿ�Ҫ�������汾��Ϸ��\n��Y���¿�ʼ  �������������������\n");
		want_to_play = getch();
	}while(want_to_play=='y'||want_to_play=='Y');
	return 0;
}