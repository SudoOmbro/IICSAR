#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

#define alt 200  //red:    \033[22;31m  //
#define lun 200  //green:  \033[22;32m  //

#define yview 10
#define xview 30

int A[alt][lun], x, y, hp, mhp, mov, lv, xp, mxp, b, npt, flr, gold, armorprice;
float heavy_attack_multiplier;


// battle menu -----------------------------------

int roll(int success_chanche)
{
	int chanche=rand()%101;
	return chanche <= success_chanche;
}

int random_range(int min, int max)
{
	return min + (rand()%(max - min + 1));
}

void battle()
{
	int enhp, turn, win, yt, act, temp, dmg, ex;
	int player_light_attack_max_damage = 4*lv;
	int player_light_attack_min_damage = player_light_attack_max_damage - lv;
	int player_heavy_attack_min_damage = player_light_attack_min_damage * heavy_attack_multiplier;
	int player_heavy_attack_max_damage = player_light_attack_max_damage * heavy_attack_multiplier;
	srand(time(NULL));
	win = turn = ex = 0;
	enhp = flr * 2 + rand()%9;
	ex += enhp;
	system("cls");
	printf("you encounter a monster!\n");
	yt = rand()%3; // determines if you start first, if 0 the enemy starts first
	while(win==0)
	{
		turn+=1;
		printf("turn %d\n\n", turn);
		printf("the monster has %d hp \nyou have %d hp\n\n", enhp, hp);
		if (yt != 0)
		{
			printf(
				"what do you want to do? \n1 = fast attack (%d - %d dmg) (95%%) \n2 = heavy attack (%d - %d dmg) (70%%) \n3 = try to flee (50 percent chance) \n4 = use potion (%d left)\n\n)", 
				player_light_attack_min_damage, 
				player_light_attack_max_damage, 
				player_heavy_attack_min_damage, 
				player_heavy_attack_max_damage, 
				npt
			);
			scanf("%d", &act);
			switch (act)
			{
				case 1:
					if (roll(95))
					{
						dmg = random_range(player_light_attack_min_damage, player_light_attack_max_damage);
						enhp -= dmg;
						printf("you inflicted %d damage to the monster!\n", dmg);
					}
					else printf("your attack has failed!\n");
					break;
				case 2:
					if (roll(95))
					{
						dmg = random_range(player_heavy_attack_min_damage, player_heavy_attack_max_damage);
						enhp -= dmg;
						printf("you inflicted %d damage to the monster!\n", dmg);
					}
					else printf("your attack has failed!\n");
					break;
				case 3:
					if (roll(50))
					{
						win=3;
						printf("you have successfully escaped!\n");
					}
					else printf("your escape attempt failed!\n");
					break;
				case 4:
					if (npt != 0)
					{
						hp=mhp;
						npt-=1;
						printf("you used a potion, you hp were maxed out!\n");
					}
					else printf("you don't have any potions!\n");
					break;
			}
			yt = 0;
		}
		if (enhp>0 && win == 0)
		{
			printf("the enemy attacks!\n");
			if (roll(85))
			{
				dmg=(2+flr)+rand()%3;
				hp-=dmg;
				printf("the enemy inflicted %d damage!\n", dmg);
			}
			else printf("the enemy has missed!\n");
			yt=1;
		}
		printf("\n");
		if (hp<=0)
		{
			win=-1;
			b=1;
			printf("you have been killed by the monster!\n");
		}
		else if (enhp<=0)
		{
			win=1;
			ex+=lv*2+rand()%21;
			int gained_gold = flr*2 + rand()%19;
			gold += gained_gold;
			xp += ex;
			printf("you won! you gained %d experience and %d gold!\n", ex, gained_gold);
			if (roll(50))
			{
				npt+=1;
				printf("the enemy also dropped you a potion!\n");
			}
		}
	}
	if (xp>=mxp)
	{
		lv+=1;
		mxp+=mxp*1.5;
		mhp+=5;
		hp=mhp;
		printf("you reached level %d! \nmax hp = %d, health restored.\n", lv, mhp);
	}
	system("pause");
}

// merchant menu --------------------------------------

bool use_gold(int needed_amount)
{
	if (gold >= needed_amount)
	{
		gold -= needed_amount;
		return true;
	}
	printf("you don't have enough gold!");
	return false;
}

void merchant()
{
	int act, done;
	done = 0;
	system("cls");
	printf("welcome to a shop!");
	while (done == 0) 
	{
		printf("\nHowdy adventurer! do you want to buy anything? (your gold: %d)\n", gold);
		printf("1 - potion - 70 gold\n");
		printf("2 - armor upgrade - %d gold (+1 to maximum hp, current: %d)\n", armorprice, mhp);
		printf("3 - sword upgrade - %d gold (+0.2 to heavy attack multiplier, current: %2.1f)\n", armorprice, heavy_attack_multiplier);
		printf("4 - goodbye!\n");
		printf("\nwhat do you want to do? ");
		scanf("%d", &act);
		printf("\n");
		switch (act)
		{
			case 1:
				if (use_gold(70))
				{
					printf("here you go!\nyou got 1 more potion and you lost 70 gold");
					npt += 1;
				}
				break;
			case 2:
				if (use_gold(armorprice))
				{
					printf("here you go!\nyou got 1 more HP and you lost %d gold", armorprice);
					mhp += 1;
					armorprice = armorprice * 1.25;
				}
				break;
			case 3:
				if (use_gold(armorprice))
				{
					printf("here you go!\nyou increased your heavy attack multiplier and lost %d gold", armorprice);
					heavy_attack_multiplier += 0.2;
					armorprice = armorprice * 1.25;
				}
				break;
			case 4:
				printf("you went away.");
				done = 1;
				break;
			default:
				printf("invalid action\n");
				break;
		}
	}
	printf("\n\nThe merchant disappears.");
	system("pause");
}

// map generation & printing -----------------------------

void start()
{
	// generate the map
	int i, j, temp, n, m, seed, times;
	temp=seed=times=0;
	printf("\ndo you want to insert a seed? (type 0 if you don't) ");
	scanf("%d", &seed);
	if (seed==0) srand(time(NULL));
	else srand(seed);
	for(i=0;i<alt;i++)
		for(j=0;j<lun;j++) A[i][j]=rand()%220;
	for(i=0;i<alt;i++)
	{
		for(j=0;j<lun;j++)
		{
			if (A[i][j]!=7) A[i][j] = 0;
		}
	}
	for(i=alt/2-alt/4;i<alt/2+alt/4;i++)
	{
		for (j=lun/2-lun/4;j<lun/2+lun/4;j++)
		{
			if (A[i][j]==7)
			{	
				temp=10+rand()%8;
				for(n=i;n<i+temp;n++) if (A[n][j]!=2 && A[n][j]!=10) A[n][j]=3;
				for(m=j;m<j+temp-3;m++) if (A[n][m]!=2 && A[n][j]!=10) A[n][m]=3;
				for(n;n>i;n--) if (A[n][m]!=2 && A[n][j]!=10) A[n][m]=3;
				for(m;m>j;m--) if (A[1][m]!=2 && A[n][j]!=10) A[i][m]=3;
				for(n=i+1;n<i+temp;n++)
				{
					for (m=j+1;m<j+temp-3;m++)
					{
						if (A[n][m]!=7 && A[n][m]!=10) A[n][m]=2;
						if (n==(i+i+temp)/2 && m==(j+j+temp-3)/2) A[n][m]=10;
					}
				}
				A[i][j]=3;	
			}
		}	
	}
	n=m=0;
	while(times < 2) {
	for(i=alt/2-alt/4;i<alt/2+alt/4;i++)
	{
		for (j=lun/2-lun/4;j<lun/2+lun/4;j++)
		{
			if (A[i][j]==10)
			{
				for(n=alt/2-alt/4;n<alt/2+alt/4;n++)
				{
					for (m=lun/2-lun/4;m<lun/2+lun/4;m++)
					{
						if (A[n][m]==10 && n!=i && m!=j)
						{
							x = n-i;
							y = m-j;
							n = m = alt + lun;
						}
					}
				}		
				if (x>0)
				{
								temp=i;
								while(x>0)
								{
									if (A[temp][j]!=2)
									{
										A[temp][j] = A[temp][j+1] = A[temp][j-1] = 2;
										if (A[temp][j+2]!=2 && A[temp][j+2]!=10) A[temp][j+2] = 3;
										if (A[temp][j-2]!=2 && A[temp][j-2]!=10) A[temp][j-2] = 3;
									}
									x--;
									if (x==0 && A[temp+1][j]!=2 && A[temp+1][j]!=10)
									{
										A[temp+1][j] = A[temp+1][j+1] = A[temp+1][j-1] = 2;
										A[temp+2][j] = A[temp+2][j+1] = A[temp+2][j-1] = A[temp+2][j-2] = A[temp+2][j+2] = A[temp+1][j-2] = A[temp+1][j+2] = 3;
									} 
									temp++;
								}
					x=temp;
				}
				if (x<0)
				{
								temp=i;
								while(x<0)
								{
									if (A[temp][j]!=2)
									{
										A[temp][j] = A[temp][j+1] = A[temp][j-1] = 2;
										 if (A[temp][j+2]!=2 && A[temp][j+2]!=10) A[temp][j+2] = 3;
										 if (A[temp][j-2]!=2 && A[temp][j-2]!=10) A[temp][j-2] = 3;
									}
									x++;
									if (x==0 && A[temp-1][j]!=2 && A[temp-1][j]!=10)
									{
										A[temp-1][j] = A[temp-1][j+1] = A[temp-1][j-1] = 2;
										A[temp-2][j] = A[temp-2][j+1] = A[temp-2][j-1] = A[temp-2][j-2] = A[temp-2][j+2] = A[temp-1][j-2] = A[temp-1][j+2] = 3;
									} 
									temp--;
								}
					x=temp;
				}
				if (y>0)
				{
								temp=j;
								while (y>0)
								{
									if (A[x][temp]!=2)
									{
										A[x][temp] = A[x+1][temp] = A[x-1][temp] = 2;
										if (A[x-2][temp]!=2 && A[x-2][temp]!=10) A[x-2][temp] = 3;
										if (A[x+2][temp]!=2 && A[x+2][temp]!=10) A[x+2][temp] = 3;
									}
									y--;
									if (y==0 && A[x][temp+1]!=2 && A[x][temp+1]!=10)
									{
										A[x][temp+1] = A[x+1][temp+1] = A[x-1][temp+1] = 2;
										A[x-2][temp+1] = A[x+2][temp+1] = A[x][temp+2] = A[x+1][temp+2] = A[x+2][temp+2] = A[x-1][temp+2] = A[x-2][temp+2] = 3;
									}
									temp++;
								}
				}
				if (y<0)
				{
								temp=j;
								while (y<0)
								{
									if (A[x][temp]!=2)
									{
										A[x][temp] = A[x+1][temp] = A[x-1][temp] = 2;
										if (A[x-2][temp]!=2 && A[x-2][temp]!=10) A[x-2][temp] = 3;
										if (A[x+2][temp]!=2 && A[x+2][temp]!=10) A[x+2][temp] = 3;
									}
									y++;
									if (y==0 && A[x][temp-1]!=2 && A[x][temp-1]!=10)
									{
										A[x][temp-1] = A[x-1][temp+1] = A[x-1][temp-1] = 2;
										A[x-2][temp-1] = A[x+2][temp-1] = A[x][temp-2] = A[x+1][temp-2] = A[x+2][temp-2] = A[x-1][temp-2] = A[x-2][temp-2] = 3;
									}
									temp--;
								}
				}
				A[i][j] = 2;
				x = y = 0;
			}
		}
	}
	times+=1;
	}
	for(i=0;i<alt;i++)
	{
		for (j=0;j<lun;j++)
		{
			if (A[i][j]==2)
			{
				temp=rand()%100;
				if (flr<7)
				{
					if (temp<5+flr) A[i][j] = 4;
				}
				else
				{
					if (temp<12) A[i][j] = 4;
				}	
			}
		}
	}
	for (i=0;i<alt;i += 5)
	{
		for (j=0;j<lun;j += 5)
		{
			if (A[i][j] == 2)
			{
				temp = rand()%100;
				if (temp < 5)
				{
					A[i][j] = 6;
				}
			}
		}
	}
	temp=0;
	while(temp==0)
	{
		x = rand()%alt;
		y = rand()%lun;
		if (A[x][y]==2) temp=1;
	}
	A[x][y] = 1;
	temp=0;
	while(temp==0)
	{
		i = rand()%alt;
		j = rand()%lun;
		if (A[i][j]==2) temp=1;
	}
	A[i][j] = 5;
	system("cls");
	printf("\nthe exit is ");
	if (i-x>9)
	{
		temp=1;
		printf("south ");
	}
	if (i-x<-9) 
	{
		temp=1;
		printf("north ");
	}
	if (j-y>22) 
	{
		temp=1;
		printf("east ");
	}
	if (j-y<-22) 
	{
		temp=1;
		printf("west ");
	}
	if (temp==0) printf("near ");
	printf("your position. (y:%d, X:%d)\n\n", i, j);
	system("pause");
}

void stamp()
{
	system("cls");
	printf("floor %d\n", flr);
	for (int i=x-yview;i<x+yview;i++)
	{
		printf("   ");
		for (int j=y-xview;j<y+xview;j++)
		{
			int tile_id = A[i][j];
			switch (tile_id)
			{
				case 0:
					printf(" ");
					break;
				case 1:
					printf("l");
					break;
				case 2:
					printf(".");
					break;
				case 3:
					printf("#");
					break;
				case 4:
					printf("O");
					break;
				case 5:
					printf("A");
					break;
				case 6:
					printf("M");
					break;
				default:
					printf(" ");
					break;
			}
		}
		printf("\n");
	}
}

// movement ---------------------------------------------

int get_tile_id(int world_x, int world_y) 
{
	return A[world_x][world_y];
}

void player_set_position(int next_x, int next_y)
{
	A[x][y]=2;
    x=next_x;
    y=next_y;
    A[x][y]=1;
    mov++;
}

void player_move(int x_increment, int y_increment)
{
	int next_x = x + x_increment;
	int next_y = y + y_increment;
	int tile_id = get_tile_id(next_x, next_y);
	switch (tile_id)
	{
		case 3:
			return;
		case 4:
			battle();
			player_set_position(next_x, next_y);
			break;
		case 5:
			printf("you reached floor %d!\n", flr+1);
			system("pause");
			flr+=1;
			start();
			break;
		case 6:
			merchant();	
			player_set_position(next_x, next_y);
			break;
		default:
			player_set_position(next_x, next_y);
			break;
	}
}

void enemy_move(int current_x, int current_y, int x_increment, int y_increment)
{
	int next_x = current_x + x_increment;
	int next_y = current_y + y_increment;
	int tile_id = get_tile_id(next_x, next_y);
	switch (tile_id)
	{
		case 1:
			A[current_x][current_y] = 2;
			battle();
			break;
		case 3:
			return;
		case 4:
			return;
		case 5:
			return;
		case 6:
			return;
		default:
			A[current_x][current_y] = 2;
			A[next_x][next_y] = 4;
			break;
	}
}

void enemymov()
{
	int m, i, j, ex, ey;
	for (i=0;i<alt;i++)
		for (j=0;j<lun;j++)
			if (A[i][j]==4)
				enemy_move(i, j, rand()%3 - 1, rand()%3 - 1);
}

// main ----------------------------------------------------------------

int main()
{
	printf("welcome to IICSAR, an ASCII rougelike - RPG (version 1.3.0)\n\ntry to get as far as you can!\nto input commands, press enter.\n\n l = your charachter\n O = enemies\n # = Walls\n A = Exit\n M = shops\n\nHave fun!\n\n");
	system("pause");
	inizio:
	int m;
	char wasd;
	b=mov=0;
	start();
	lv=npt=flr=1;
	xp=0;
	gold=100;
	mxp=100;
	hp=10;
	mhp=10;
	armorprice = 100;
	heavy_attack_multiplier = 2;
	stamp();
	while(b==0)
	{
		printf("\nhp: %d/%d lv: %d exp: %d/%d potions: %d  pos: y %d, x %d  gold: %d  \n", hp, mhp, lv, xp, mxp, npt, x, y, gold);
		printf("\nwhere do you want to move? (W=up S=down D=right A=left) \npress P to use a potion\n");
		switch (_getch())
		{
			case 'w':
				player_move(-1, 0);
				break;
			case 'a':
				player_move(0, -1);
				break;
			case 's':
				player_move(1, 0);
				break;
			case 'd':
				player_move(0, 1);
				break;
			case 'p':
			{
				if (npt>0)
				{
					npt -= 1;
					hp = mhp;
					printf("you used a potion, your hp were maxed out!\n");
					system("pause");
				}
				else 
				{
					printf("you have no potions!\n");
					system("pause");
				}
				break;
			}
			case 'r':
				goto inizio;
				break;
		}
		enemymov();
		stamp();
		if (hp<=0) b=1;
	}
	printf("\nyou are dead!\n\n");
	system("pause");
	goto inizio;
}
