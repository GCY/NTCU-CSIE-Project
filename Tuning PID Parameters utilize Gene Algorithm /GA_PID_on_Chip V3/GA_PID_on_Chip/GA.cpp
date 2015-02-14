#include "GA.h"

Gene population[ARRAY_MAX] = {0};	    //母代
Gene pool[ARRAY_MAX] = {0};			//交配池
Gene best_gene = {0};			//選出最好的基因
int current_itera = 1;          //目前世代

float CROSSOVER_RATE = 0.6;
float MUTATION_RATE = 0.15;
unsigned int MAX = 30;
unsigned int ITERA = 100;
double input = 0,output = 0,setpoint = 120.0;
PID pid(&input,&output,&setpoint,1.7f,1.15f,0.05f,1000000.0f,ON,DIRECT,0,255);

void InitGene()
{
	unsigned int base = 1;

	for(int i = 0;i < (GENE_LENGTH / PIECE_NUM);++i){
		base *= 2;
	}

	srand(time(NULL));
	for(int i = 0;i < MAX;++i){
		unsigned int temp = 0;
		for(int j = 0;j < PIECE_NUM;++j){
			unsigned int r = rand(base+1);
			r <<= (GENE_LENGTH / PIECE_NUM) * j;
			temp |= r;
		}
		population[i].DNA = temp;
	}
}

void SwapBit(Gene *a, Gene *b, int bit_no1, int bit_no2){      //交換a,b的第bit_no個位元
    int i, tempa=a->DNA, tempb=b->DNA;
    tempa >>= GENE_LENGTH-bit_no1;
    tempb >>= GENE_LENGTH-bit_no2;
    if((tempa&=0x1) != (tempb&0x1)){
        int temp1=1, temp2=1;
        for(i=0; i<GENE_LENGTH-bit_no1; i++){temp1*=2;}
        for(i=0; i<GENE_LENGTH-bit_no2; i++){temp2*=2;}
        if(tempa==0){
            a->DNA+=temp1; b->DNA-=temp2;
        }else{
            a->DNA-=temp1; b->DNA+=temp2;
        }
    }
}
void showDNA(int dna){                   //印出DNA(big-endian)
    int j;
    for( j = GENE_LENGTH - 1; j >= 0; j--){
        printf("%d", (dna >> j) & 0x1);
    }
//    printf("\n");
}

void Crossover()
{
	//選擇：輪盤式
	double fitness_accumu_rate[MAX];     //母代適應值機率累計
    double fitness_sum = 0.0;            //母代適應值總和
    double random_rate;                  //隨機機率0~1

    for(int i = 0; i < MAX; i++) {       //母代適應值總和
       fitness_sum += population[i].error_total;
    }

    fitness_accumu_rate[0] = population[0].error_total / fitness_sum;
    for(int i = 1; i < MAX; i++) {       //母代適應值機率累計
       fitness_accumu_rate[i] =
            fitness_accumu_rate[i-1] + population[i].error_total / fitness_sum;
    }

    for(int i = 0; i < MAX; i++) {       //複製到pool
        random_rate = rand(9999);
        random_rate *= 0.0001;           //隨機機率0~1
        for(int position = 0; position < MAX; position++){
            if(random_rate <= fitness_accumu_rate[position]){
                memcpy(&pool[i], &population[position], sizeof(Gene));
                break;
            }
        }
    }

    //交配：二點交配
    int counter = 0;
    int position1, position2, point_switch1, point_switch2; //position為基因
    double random_cross_rate;                               //point_switch為要交配的位元

    while(counter < MAX){
        position1 = rand(MAX) - 1;                          //挑選不重複基因*2
        do{ position2 = rand(MAX) - 1; }
        while(position1 == position2);

        random_cross_rate = rand(9999);
        random_cross_rate *= 0.0001;                        //交配隨機機率0~1

        if(random_cross_rate <= CROSSOVER_RATE){            //決定交配
            point_switch1 = rand(GENE_LENGTH) - 1;          //挑選基因*2
            do{ point_switch2 = rand(GENE_LENGTH) - 1; }
            while(point_switch1 == point_switch2);

            memcpy(&population[counter], &pool[position1], sizeof(Gene));
            memcpy(&population[counter+1], &pool[position2], sizeof(Gene));

            SwapBit(&population[counter], &population[counter+1], point_switch1, point_switch2); //二點交配
            SwapBit(&population[counter], &population[counter+1], point_switch2, point_switch1);

            counter += 2;                                   //兩條完成複製
        }else{                                              //決定不交配
            memcpy(&population[counter], &pool[position1], sizeof(Gene));
            memcpy(&population[counter+1], &pool[position2], sizeof(Gene));

            counter += 2;                                   //兩條完成複製
        }
    }

    current_itera++;                                        //進入下個世代
}

void Mutation()                                             //突變：單點突變
{
	int counter = 0;
	int mutation_bits_num = MAX * GENE_LENGTH * MUTATION_RATE;//突變bit數
	int random_gene, random_bit;                            //隨機基因、位元

	while(counter < mutation_bits_num){                     //突變位元
        random_gene = rand(MAX) - 1;
        random_bit = rand(GENE_LENGTH) - 1;

        int i, temp = population[random_gene].DNA;
        temp >>= GENE_LENGTH - random_bit;

        temp &= 0x1;
        int factor = 1;
        for(i=0; i<GENE_LENGTH-random_bit; i++){factor*=2;}
        if(temp==0){
            population[random_gene].DNA+=factor;
        }else{
            population[random_gene].DNA-=factor;
        }

        counter++;
	}

    memcpy(&population[0], &best_gene, sizeof(Gene)); //保留最佳母體
}

void ForTestFitness()                       //單純測試用，看12位bit，愈小愈好
{
    int max=0;            //目前Fitness最大值
    int best_gene_position = 0; //最好的基因位置
    int error = 0;              //PID回傳結果
    int temp;
    int Magnification = 1000;    //放大倍率

    for(int i = 0;i < MAX;i++) {
       temp = population[i].DNA & 0xFFF;//*************模擬：看12位bit，愈小愈好*****************
       if(temp !=0 )            //取誤差值倒數
            population[i].error_total = (unsigned long)Magnification / temp;
       if(max < population[i].error_total) {
            max = population[i].error_total;
            best_gene_position = i;
       }
    }
    best_gene = population[best_gene_position]; //選出最好基因

}

void FitnessFunc()              //計算Fitness，愈大愈好
{
    int max = 0;                //目前Fitness最大值
    int best_gene_position = 0; //最好的基因位置
    int error = 0;              //PID回傳結果
    int Magnification = 100;    //放大倍率

    for(int i = 0;i < MAX;i++) {
       error = _PID(&population[i]);
       if(error != 0)           //取誤差值倒數
            population[i].error_total = (unsigned long)Magnification / error;
       if(max < population[i].error_total) {
            max = population[i].error_total;
            best_gene_position = i;
       }
    }
    best_gene = population[best_gene_position]; //選出最好基因
}

void GeneticAlgorithm()
{
	InitGene();
	Crossover();
    Mutation();
}

/**************

這個函數模擬PID跑一百次後得到總錯誤量，
寫交配跟突變函數的人要用到這個測試

傳入要跑的基因結構就可以了

**************/
unsigned long _PID(Gene *gene)
{
  unsigned long error_total = 0;
  analogWrite(3,255);
  pid.SetTunings(gene->DNA & 0x01F,
  (gene->DNA >> 5) & 0x01F,
  (gene->DNA >> 10) & 0x01F);
  //pid.SetOnOff(ON);
  for(int i = 0;i < ((22 * (gene->DNA >> 10)) + 300);++i){
    input = analogRead(A0) / 4;
    pid.Computing();
    analogWrite(3,output);
    error_total += abs(input - setpoint);
  }
  //pid.SetOnOff(OFF);
  error_total /= ((22 * (gene->DNA >> 10)) + 300);
  return error_total;
}

/**************

放到Arduino的AVR Compiler記得砍掉main

**************/
/*
int main(void)
{
//	GeneticAlgorithm();
//	FitnessFunc();

//	for(int i = 0;i < MAX;++i){
//		int temp = population[i].DNA;
//		for(int j = 0;j < GENE_LENGTH;++j){
//			std::cout << (temp & 0x1);
//			temp >>= 1;
//		}
//		std::cout << std::endl;
//	}

    InitGene();
    ForTestFitness();
    if(current_itera==1){
        std::cout << "--------------1st------" << "best Fitness:"
            << best_gene.error_total << "-------" << std::endl;
//        for(int i = 0; i < MAX; i++){
//            showDNA(population[i].DNA);
//            std::cout << "-----" << population[i].error_total << std::endl;
//        }
    }

    while(current_itera<=100){
        Crossover();
        Mutation();
        ForTestFitness();
        std::cout << "-------" << current_itera << "nd----best Fitness:"
            << best_gene.error_total << "-------" <<std::endl;
//        for(int i = 0; i < MAX; i++){
//            showDNA(population[i].DNA);
//            std::cout << "-----" << population[i].error_total << std::endl;
//        }
    }

	getchar();
	return 0;
}
*/
