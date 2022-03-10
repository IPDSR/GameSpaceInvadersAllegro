#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define MAX_ALIENS 35
#define MAX_PROJETIL 500

const float FPS = 100;  //frames por segundo

const int SCREEN_W = 960;	//largura da tela
const int SCREEN_H = 540;	//altura da tela

const int GRASS_H = 60;		//altura da grama do cenario

const int NAVE_W = 100;		//largura da nave
const int NAVE_H = 50;		//altura da nave

const int ALIEN_W = 50;		//largura dos aliens
const int ALIEN_H = 25;		//altura dos aliens

const int ESPECIAL_WH = 40;	//altura e largura do alien especial

//Declaracao de variaveis globais----------------------------------------------------------------------------------------------------------------------------

int record = 0, score = 0, a = 0, b = 245, c = 0, d = 0;	//recorde e pontuacao
float velocity = 0;	//velocidade que é incrementada em 0.5 a cada fase concluida
char my_score[100] = {0};	//string para armazenar texto

ALLEGRO_FONT *size_f;   //tamanho da fonte

//Structs das estruturas utilizadas no jogo------------------------------------------------------------------------------------------------------------------

typedef struct Nave {		//caracteristicas da nave
	float x;
	float vel;
	int dir, esq;
	ALLEGRO_COLOR cor;
}	Nave;

typedef struct Alien {		//caracteristicas dos aliens
	float x, y;
	float x_vel, y_vel;
	ALLEGRO_COLOR cor;
} Alien;

typedef struct Especial {		//caracteristicas do alien principal
	float x, y;
	float vel;
	ALLEGRO_COLOR cor;
} Especial;

typedef struct Projetil	{		//caracteristicas do projetil
	float x, y;
	float vel;
	ALLEGRO_COLOR cor;
} Projetil;

//Rotinas de inicializacao e atualizacao das estruturas e do cenario, e atualizacao do recorde e pontuacao

void initProjetil (Projetil *projetil, Nave *nave) { 		//inicializacao do projetil
	projetil->x = nave->x;
	projetil->y = SCREEN_H - GRASS_H - NAVE_H/2;
	projetil->vel = 4;
	projetil->cor = al_map_rgb(255, 255, 255);
}

void draw_projetil (Projetil projetil)		//desenha o projetil
{
	al_draw_filled_circle(projetil.x, projetil.y, 2.5, projetil.cor);

}

void update_projetil (Projetil *projetil)	//atualiza a posicao do projetil
{
	projetil->y = projetil->y - projetil->vel;

}
void initNave(Nave *nave) {		//inicializa nave
	nave->x = SCREEN_W/2;
	nave->vel = 1.5 + velocity;
	nave->dir = 0;
	nave->esq = 0;
	nave->cor = al_map_rgb(0, 0, 255);
}

void initAlien(Alien *alien, int i)		//inicializa todos os aliens
{
	if (i <= 6){
			alien->x = 2*i*ALIEN_W;
			alien->y = 0;
	}
	if (i >= 7 && i <=13){
			alien->x = 2*(i-7)*ALIEN_W;
			alien->y = 2.5*ALIEN_H;
	}
	if (i >= 14 && i <=20) {
			alien->x = 2*(i-14)*ALIEN_W;
			alien->y = 5*ALIEN_H;
	}
	if (i >= 21 && i <=27){
			alien->x = 2*(i-21)*ALIEN_W;
			alien->y = 7.5*ALIEN_H;
	}
	
	if (i >= 28 && i <=34){
			alien->x = 2*(i-28)*ALIEN_W;
			alien->y = 10*ALIEN_H;
	}

	alien->x_vel = 0.5 + velocity;
	alien->y_vel = ALIEN_H;
	alien->cor = al_map_rgb(rand()%256, rand()%256, rand()%256);
}

void draw_alien (Alien alien)	//desenha os aliens
{
	al_draw_filled_rectangle(alien.x, alien.y, alien.x + ALIEN_W, alien.y + ALIEN_H, alien.cor);
}

void update_alien(Alien *alien) {	//atualiza a posicao dos aliens
	if (alien->x + ALIEN_W + alien->x_vel > SCREEN_W || alien->x + alien->x_vel < 0)
	{		
		alien->y = alien->y + alien->y_vel;															
		alien->x_vel = alien->x_vel * (-1);
	}
	
	alien->x = alien->x + alien->x_vel;
}

void initEspecial(Especial *especial)	//inicializa o alien especial
{
	especial->x = 0;
	especial->y = 30;
	especial->vel = 1;
	especial->cor = al_map_rgb(255, 0, 0);
}

void draw_Especial (Especial especial)	//desenha o alien especial
{
	al_draw_filled_rectangle(especial.x, especial.y, especial.x + ESPECIAL_WH, especial.y + ESPECIAL_WH, especial.cor);
}

void update_Especial (Especial *especial)	//atualiza a posicao do alien especial
{
	especial->x = especial->x + especial->vel;
}

void draw_scenario(){		//desenha o cenario
		al_clear_to_color(al_map_rgb(a, c, d));
		al_draw_filled_rectangle(0, SCREEN_H-GRASS_H, SCREEN_W, SCREEN_H, al_map_rgb(a, b, c));
}

void draw_nave (Nave nave) {	//desenha a nave
	float y_base = SCREEN_H - GRASS_H/2;
	al_draw_filled_triangle(nave.x, y_base - NAVE_H, nave.x - NAVE_W/2, y_base, nave.x + NAVE_W/2, y_base, nave.cor);
	sprintf(my_score, "SCORE: %d", score);
	al_draw_text(size_f, al_map_rgb(255, 255, 255), 5, 5, 0, my_score); 
	sprintf(my_score, "RECORD: %d", record);
	al_draw_text(size_f, al_map_rgb(255, 255, 255), 700, 5, 0, my_score); 	
}

void update_nave (Nave *nave)		//atualiza a posicao da nave
{
	if(nave->dir && nave->x + nave->vel<=SCREEN_W) {
		nave->x = nave->x + nave->vel;
	}
	if(nave->esq && nave->x - nave->vel>=0) {
		nave->x = nave->x - nave->vel;
	}
}

void newRecord(int game_mode) {		//le e atualiza os recordes nos dois arquivos txt
	if (game_mode == 1)
	{
	FILE *arq = fopen("recorde_one.txt", "r");
	if(arq != NULL) {
		fscanf(arq, "%d", &record);
		fclose(arq);
	}
	if(record < score) {
		arq = fopen("recorde_one.txt", "w");
		fprintf(arq, "%d", score);
		fclose(arq);
	}
	}
	
	else
	{
	FILE *arq = fopen("recorde_mult.txt", "r");
	if(arq != NULL) {
		fscanf(arq, "%d", &record);
		fclose(arq);
	}
	if(record < score) {
		arq = fopen("recorde_mult.txt", "w");
		fprintf(arq, "%d", score);
		fclose(arq);
	}
	}
}

//Funcao principal que faz o jogo funcionar------------------------------------------------------------------------------------------------------------------

int main(){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	
	ALLEGRO_SAMPLE_ID id_music;		//id dos samples que tocam em loop
	ALLEGRO_SAMPLE_ID id_special;
	ALLEGRO_SAMPLE_ID id_aliens;
	
	ALLEGRO_SAMPLE *shot = NULL;	//declaracao dos samples
	ALLEGRO_SAMPLE *death_invaders = NULL;
	ALLEGRO_SAMPLE *game_over = NULL;
	ALLEGRO_SAMPLE *music = NULL;
	ALLEGRO_SAMPLE *next = NULL;
	ALLEGRO_SAMPLE *special = NULL;
	ALLEGRO_SAMPLE *aliens = NULL;
	ALLEGRO_SAMPLE *beginning = NULL;
	ALLEGRO_SAMPLE *pause = NULL;
	ALLEGRO_SAMPLE *end = NULL;

	Alien alien[MAX_ALIENS];		//declaracao das estruturas, e de algumas variaveis necessarias
	Projetil projetil;
	Projetil sprojetil[MAX_PROJETIL];
	Especial especial;
	projetil.y = -52000;	//para garantir que o projetil nao comecara interferindo na tela do jogo		
	especial.y = -50000;																			//j = indice projetil, pont determina se diminui ou nao os pontos
	int aliens_abatidos = 0, time = 0, pontos_aliens = 50, pontos_especial = 200, game_pause = 1, game_mode, j = 0, pont, select = 0, cheat = 0;// permite ou nao a escolha do modo de jogo
   
	//----------------------- rotinas de inicializacao ------------------------------------------------------------------------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}
	
	    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}	

	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	
		//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
		//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}
	
	//instala audio
	  if (!al_install_audio())
	  {
		fprintf(stderr, "Falha ao inicializar áudio.\n");
		return -1;
	  }
	 //instala codec de audio
	  if (!al_init_acodec_addon())
	  {
		fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
		return -1;
	  }
	  //instala espacos reservados para os samples
	  if (!al_reserve_samples(10))
	  {
		fprintf(stderr, "Falha ao alocar canais de áudio.\n");
		return -1;
	  }
	  
	  shot = al_load_sample("Shot.wav");	//carrega o som na variavel
	  if (!shot)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  pause = al_load_sample("Pause.wav");		//carrega o som na variavel
	  if (!pause)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  game_over = al_load_sample("Game_Over.wav");		//carrega o som na variavel
	  if (!game_over)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  aliens = al_load_sample("Aliens.wav");		//carrega o som na variavel
	  if (!aliens)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");		
		return -1;
	  }
	  
	  music = al_load_sample("Music.wav");		//carrega o som na variavel
	  if (!music)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  death_invaders = al_load_sample("Death_Invaders.wav");		//carrega o som na variavel
	  if (!death_invaders)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  next = al_load_sample("Next.wav");		//carrega o som na variavel
	  if (!next)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  special = al_load_sample("Special.wav");		//carrega o som na variavel
	  if (!special)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  beginning = al_load_sample("Beginning.wav");		//carrega o som na variavel
	  if (!beginning)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	  
	  end = al_load_sample("End.wav");		//carrega o som na variavel
	  if (!end)
	  {
		fprintf(stderr, "Falha ao carregar sample.\n");
		return -1;
	  }
	
	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));	
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));


	//inicializa o modulo que permite carregar imagens no jogo
	al_init_image_addon();

	//inicializa o modulo allegro que carrega as fontes
    al_init_font_addon();
	//inicializa o modulo allegro que entende arquivos tff de fontes
    al_init_ttf_addon();
	//carrega o arquivo arial.ttf da fonte PressStart2P e define que sera usado o tamanho 20 (segundo parametro)
    size_f = al_load_font("PressStart2P.ttf", 20, 1);
		
	//colore toda a tela de preto
	al_clear_to_color(al_map_rgb(0,0,0));
	
	//carrega a imagem e a atribui a variavel black
	ALLEGRO_BITMAP *black = al_load_bitmap("black.jpg");
	//carrega a imagem e a atribui a variavel image
	ALLEGRO_BITMAP *image = al_load_bitmap("spaceinvaders.png");
	//coloca na tela a imagem armazenada na variavel image
	al_draw_bitmap(image, 0, 0, 0);	
	
	sprintf(my_score, "PRESS 1 FOR GAME MODE ONE SHOT");
	al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 300, SCREEN_H/2 + 65, 0, my_score);
	sprintf(my_score, "PRESS 2 FOR GAME MODE MULTIPLE SHOT");
	al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 350, SCREEN_H/2 + 100, 0, my_score);
	al_play_sample(music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_music);	

	//reinicializa a tela
	al_flip_display();		 
	
	Nave nave;
	initNave(&nave);
	
	for (int i = 0; i < MAX_ALIENS; i++)	//Inicializa todos 35 os aliens
	{
		initAlien(&alien[i], i);
	}
	int playing = 1;	

	
	//inicia o temporizador
	al_start_timer(timer);
	
//Tudo que acontece durante o jogo-----------------------------------------------------------------------------------------------------------------------------

	while (playing)		 //enquanto esta jogando
	{
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);
		
		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {
			//jogo não está em pause
			if (game_pause == 0)
			{
					
				draw_scenario();	//desenha o cenario
				
				newRecord(game_mode);
				
				update_nave(&nave);		//atualiza a posicao da nave
				
				draw_nave(nave);	//	desenha a nave na nova posicao
				
				for (int i = 0; i < MAX_ALIENS; i++)	//desenha  todos os aliens nas novas posicoes e atualiza as posicoes
				{
					if (cheat == 0) 
					{
						if (alien[i].x + ALIEN_W + alien[i].x_vel > SCREEN_W || alien[i].x + alien[i].x_vel < 0)
							{
								for (int j = 0; j < MAX_ALIENS; j++)
								{
									alien[j].y = alien[j].y + alien[j].y_vel;	
									alien[j].x_vel = alien[j].x_vel * (-1);
								}
							}
							alien[i].x = alien[i].x + alien[i].x_vel;
						//update_alien(&alien[i]);
					}
						draw_alien(alien[i]);
					
				}
				
				
				if (game_mode == 1)	//para o modo de jogo one shot
				{
					update_projetil(&projetil);		//atualiza a posicao do projetil
					draw_projetil(projetil);		//desenha o projetil na nova posicao
					for (int k = 0; k < MAX_ALIENS; k++)	//varredura em todos os aliens
					{
							//se o projetil encosta em algum alien, tanto o projetil quanto o alien sao jogados para fora do mapa, entao na pratica o alien
							//deixa de existir, enquanto o projetil poder ser lancado novamente
						if ((projetil.x > alien[k].x) && (projetil.x < alien[k].x + ALIEN_W) && (projetil.y > alien[k].y) && (projetil.y < alien[k].y + ALIEN_H))
						{
							alien[k].y = -50000;
							projetil.y = -52000;
							aliens_abatidos = aliens_abatidos + 1;
							score = score + pontos_aliens;
							al_play_sample(death_invaders, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							
							 
						}
					}
					
					//se o projetil encosta no alien especial, acontece o mesmo que foi descrito com os aliens comuns
					if ((projetil.x > especial.x) && (projetil.x < especial.x + ESPECIAL_WH) && (projetil.y > especial.y) && (projetil.y <especial.y + ESPECIAL_WH))
					{
						especial.y = -50000;
						projetil.y = -52000;
						al_stop_sample(&id_special);
						score = score + pontos_especial;
						al_play_sample(death_invaders, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				else
				{
					for (int z = 0; z <= j; z++)
					{
						update_projetil(&sprojetil[z]);		//atualiza a posicao do projetil
						draw_projetil(sprojetil[z]);		//desenha o projetil na nova posicao
					}
					
					for (int z = 0; z < j; z++)
					{
						for (int k = 0; k < MAX_ALIENS; k++)	//varredura em todos os aliens
						{
								//se algum dos projeteis encosta em algum alien, tanto o projetil quanto o alien sao jogados para fora do mapa, entao na pratica o alien
								//deixa de existir
							if ((sprojetil[z].x > alien[k].x) && (sprojetil[z].x < alien[k].x + ALIEN_W) && (sprojetil[z].y > alien[k].y) && (sprojetil[z].y < alien[k].y + ALIEN_H))
							{
								alien[k].y = -50000;
								sprojetil[z].y = -52000;
								aliens_abatidos = aliens_abatidos + 1;
								score = score + pontos_aliens;
								al_play_sample(death_invaders, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							}
						}
						
							//se algum dos projeteis encosta no alien especial, acontece o mesmo que foi descrito com os aliens comuns
						if ((sprojetil[z].x > especial.x) && (sprojetil[z].x < especial.x + ESPECIAL_WH) && (sprojetil[z].y > especial.y) && (sprojetil[z].y <especial.y + ESPECIAL_WH))
						{
							especial.y = -50000;
							sprojetil[z].y = -52000;
							al_stop_sample(&id_special);
							score = score + pontos_especial;
							al_play_sample(death_invaders, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
					}
					
					if (j%5 == 0 && j != 0 && pont == 0)	//decresce o valor do alien comum no modo multiple shot a medida que sao efetuados muitos disparos
					{
						pontos_aliens--;
						pont = 1;
					}
				}
				
				if (aliens_abatidos == MAX_ALIENS) //se todos os aliens foram abatidos, muda o cenario, aumenta a dificuldade, reinicia os aliens, chama a proxima "fase"
				{
					if (especial.x < SCREEN_W)
						al_stop_sample(&id_special);
					al_stop_sample(&id_aliens);
					al_play_sample(next, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					al_draw_bitmap(black, 0, 0, 0);
					sprintf(my_score, "DIFFICULTY INCREASED");
					al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 200, SCREEN_H/2, 0, my_score);
					al_flip_display();
					al_rest(4);
					especial.y = -10000;
					aliens_abatidos = 0;
					j = 0;
					pontos_aliens = 50;
					cheat = 0;
					a = rand()%256;
					b = rand()%256;
					c = rand()%256;
					d = rand()%256;
					velocity = velocity + 0.25;
					initNave(&nave);
					al_play_sample(aliens, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_aliens);
					for (int i = 0; i < MAX_ALIENS; i++)	//Inicializa todos 28 os aliens
					{
						initAlien(&alien[i], i);
					}
				}
				
				for (int k = 0; k < MAX_ALIENS; k++)	//varredura em todos os aliens
				{ 
						//se o alien encosta na ponta da nave ou chega muito perto
					if ((nave.x + NAVE_W/4> alien[k].x) && (nave.x - NAVE_W/4 < alien[k].x + ALIEN_W) && (SCREEN_H - GRASS_H/2 - NAVE_H - 5 < alien[k].y + ALIEN_H))
					{
						al_draw_filled_triangle(nave.x, SCREEN_H - GRASS_H/2 - NAVE_H, nave.x - NAVE_W/2, SCREEN_H - GRASS_H/2, nave.x + NAVE_W/2, SCREEN_H - GRASS_H/2, al_map_rgb(255, 0, 0));
					}
					
					if (alien[k].y + ALIEN_H > SCREEN_H - GRASS_H) //se o alien encosta na grama, substuindo a funcao colisao_alien_solo do professor
					{
						al_draw_filled_rectangle(0, SCREEN_H-GRASS_H, SCREEN_W, SCREEN_H, al_map_rgb(255, 0, 0));
					}
					
					if((alien[k].y + ALIEN_H > SCREEN_H - GRASS_H) || ((nave.x + NAVE_W/4> alien[k].x) && (nave.x - NAVE_W/4 < alien[k].x + ALIEN_W) && (SCREEN_H - GRASS_H/2 - NAVE_H - 5 < alien[k].y + ALIEN_H)))
					{
						al_play_sample(end, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_filled_rectangle(alien[k].x, alien[k].y, alien[k].x + ALIEN_W, alien[k].y + ALIEN_H, al_map_rgb(255, 0, 0));
						al_flip_display();
		
						if (especial.x < SCREEN_W)
							al_stop_sample(&id_special);
						playing = 0; //para o jogo, uma pequena animacao para identificar por que o jogo acabou
						al_rest(1);
					}
				}
				
				if (aliens_abatidos != 0 && (aliens_abatidos%10 == 0) && (especial.y < 0 || especial.x > SCREEN_W))	//alien especial eh inicializado
				{																									//a cada 10 aliens comuns abatidos
					initEspecial(&especial);
					al_play_sample(special, 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_special);
				}
				
				if (especial.x == SCREEN_W)		//quando o especial sai da tela, a musica para de tocar
				{
					al_stop_sample(&id_special);
				}
				
				update_Especial(&especial);		//atualiza a posicao do especial
				draw_Especial(especial);
				
				if (time%5 == 0 && time != 0 && al_get_timer_count(timer)%(int)FPS == 0)	//valor do alien comum decresce a cada 5 segundos
				{
					pontos_aliens--;
				}
			}
		}

		//atualiza a tela (quando houver algo para mostrar)
		al_flip_display();
		
		if(al_get_timer_count(timer)%(int)FPS == 0)
		{
			printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
			time = (int)(al_get_timer_count(timer)/FPS);
		}
		
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			if (especial.x < SCREEN_W)
				al_stop_sample(&id_special);
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
			
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_A: 
					nave.esq = 1;
				break;
					
				case ALLEGRO_KEY_D:
					nave.dir = 1;
				break;
				case ALLEGRO_KEY_SPACE: 
					if (game_mode == 1)
					{
						if (projetil.y < 0)
						{
							initProjetil(&projetil, &nave);
							al_play_sample(shot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);	//lanca o projetil e a musica no modo oneshot
						}
					}
					else{
						initProjetil(&sprojetil[j], &nave);
						al_play_sample(shot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);	//lanca inumeros projeteis e a musica no modo multiple shot
						j++;
						pont = 0;
					}
				break;
				case ALLEGRO_KEY_ESCAPE: //implementei o pause aqui dentro, pois assim so realiza as acoes a cada vez que aperta, se fosse no playing ficaria
										//em loop e prejudicaria o desempenho do jogo
					game_pause = !game_pause;
					al_play_sample(pause, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					if (game_pause)
					{
					//coloca na tela a imagem armazenada na variavel image
					al_draw_bitmap(black, 0, 0, 0);
					sprintf(my_score, "PAUSE");
					al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 70, SCREEN_H/2 - 30, 0, my_score);
					sprintf(my_score, "PRESS ESC TO RETURN");
					al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 200, SCREEN_H/2 + 20, 0, my_score);
					al_stop_sample(&id_aliens);
						if (especial.y > 0)
							al_stop_sample(&id_special);
					}
					else
					{
						al_play_sample(aliens, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_aliens);
						if (especial.y > 0 && especial.x < SCREEN_W)
							al_play_sample(special, 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_special);
					}
				break;
				case ALLEGRO_KEY_1:	//seleciona o modo de jogo oneshot somente na tela inicial, toca o inicio do jogo e inicializa as rotinas e variaveis necessarias
					if (select == 0)
					{
						al_stop_sample(&id_music);
						al_play_sample(beginning, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_bitmap(black, 0, 0, 0);
						sprintf(my_score, "GAMEMODE ONE SHOT");
						al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 170, SCREEN_H/2, 0, my_score);
						al_flip_display();
						game_mode = 1;
						game_pause = 0;
						select = 1;
						al_rest(3);
						al_play_sample(aliens, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_aliens);
					}
				break;
				
				case ALLEGRO_KEY_2:	//seleciona o modo de jogo multiple somente na tela inicial, toca o inicio do jogo e inicializa as rotinas e variaveis necessarias
					if (select == 0)
					{
						al_stop_sample(&id_music);
						al_play_sample(beginning, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						al_draw_bitmap(black, 0, 0, 0);
						sprintf(my_score, "GAMEMODE MULTIPLE SHOT");
						al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 220, SCREEN_H/2, 0, my_score);
						al_flip_display();
						game_mode = 0;
						game_pause = 0;
						select = 1;
						al_rest(3);
						al_play_sample(aliens, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_aliens);
					}
				break;
				case ALLEGRO_KEY_P:
					cheat = !cheat;
				break;
			}
		}
			//se o tipo de evento for um coltar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
			
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_A: 
					nave.esq = 0;
				break;
					
				case ALLEGRO_KEY_D:
					nave.dir = 0;
				break;
			}
		}
	}
	//final do jogo -----------------------------------------------------------------------------------------------------------------------------------------
	//game over, mostra a pontuacao alcancada, o record, toca a musica de game over, e finaliza o jogo
	
	al_draw_bitmap(black, 0, 0, 0);
	sprintf(my_score, "GAME OVER");
	al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 100, SCREEN_H/2 - 30, 0, my_score);
	sprintf(my_score, "SCORE: % d          RECORD: %d", score, record);
	al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W/2 - 340, SCREEN_H/2 + 20, 0, my_score);
	al_flip_display();
	al_stop_sample(&id_aliens);
	al_play_sample(game_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	al_rest(4);

 	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_sample(shot);
    al_destroy_sample(pause);	
	al_destroy_sample(death_invaders);
    al_destroy_sample(beginning);	
	al_destroy_sample(music);
    al_destroy_sample(game_over);	
	al_destroy_sample(special);
    al_destroy_sample(next);	
	al_destroy_sample(aliens);

   
   return 0;
}