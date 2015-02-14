#include "GA.h"

Gene population[MAX] = {0};	    //母代
Gene pool[MAX] = {0};			//交配池
Gene best_gene = {0};			//選出最好的基因
int current_itera = 1;          //目前世代
int calculation_num = 0;        //計算Fitness計算總次數(驗證pid_run是否有節省時間)




//目標值
int setpoint    =   0;
//PID三參數
int P = 0, I = 0, D = 0;
//
#if 0

int mask_for_verify = 0x7F;

void showDNA(int dna){                   //印出DNA(big-endian)
    int j;
    for( j = GENE_LENGTH - 1; j >= 0; j--){
        printf("%d", (dna >> j) & 0x1);
    }
//    printf("\n");
}

void ForTestFitness()                       //單純測試用，看12位bit，愈小愈好
{
    int max=0;                              //目前Fitness最大值
    int best_gene_position = 0;             //最好的基因位置
    int error = 0;                          //PID回傳結果
    int temp;
    int Magnification = 1000;               //放大倍率

    for(int i = 0;i < MAX;i++) {
        if(population[i].pid_run == 0){     //跳過母體基因為｢已經運算｣
           temp = population[i].DNA & 0xFFF;//*************模擬：看12位bit，愈小愈好*****************
           if(temp !=0 )                    //取誤差值倒數
                population[i].error_total = (unsigned long)Magnification / temp;

           population[i].pid_run = 1;       //計算完，設為｢已經運算｣
           calculation_num++;
        }
           if(max < population[i].error_total) {
                max = population[i].error_total;
                best_gene_position = i;
           }
    }
    best_gene = population[best_gene_position]; //選出最好基因
}

void ForVerify()                       //單純測試用
{
    int max=0;                              //目前Fitness最大值
    int best_gene_position = 0;             //最好的基因位置
    int error = 0;                          //PID回傳結果
    int temp, a, b;
    int Magnification = 1000000000;               //放大倍率

    for(int i = 0;i < MAX;i++) {
        if(population[i].pid_run == 0){     //跳過母體基因為｢已經運算｣
            a = population[i].DNA & mask_for_verify;
            b = (population[i].DNA >> 6) & mask_for_verify;
            temp = 19*a - 7*b;
            int distence = (temp > 789)? temp-789: 789-temp;
            temp = 154*a + 169*b;
            distence += (temp > 12345)? temp-12345: 12345-temp;

            if(distence !=0 )                    //取誤差值倒數
                population[i].error_total = (unsigned long)Magnification / distence;

            population[i].pid_run = 1;       //計算完，設為｢已經運算｣
            calculation_num++;
        }
        if(max < population[i].error_total) {
            max = population[i].error_total;
            best_gene_position = i;
        }
    }
    best_gene = population[best_gene_position]; //選出最好基因
}



void ForVerify_Main(){
    InitGene();
    ForVerify();
    if(current_itera==1){
        std::cout << "--------------1st------"
            << "best Fitness:" << best_gene.error_total
            << "-------DNA:X=" << (best_gene.DNA & mask_for_verify)
            << ", Y=" << ((best_gene.DNA>>6) & mask_for_verify) << std::endl;
//        for(int i = 0; i < MAX; i++){
//            showDNA(population[i].DNA);
//            std::cout << "-----" << population[i].error_total << std::endl;
//        }
    }

    while(current_itera<ITERA){
        Crossover();
        Mutation();
        ForVerify();
        std::cout << "-------" << current_itera << "nd----best Fitness:"
            << best_gene.error_total << "-------DNA:X=" << (best_gene.DNA & mask_for_verify)
            << ", Y=" << ((best_gene.DNA>>6) & mask_for_verify) << std::endl;
//        for(int i = 0; i < MAX; i++){
//            showDNA(population[i].DNA);
//            std::cout << "-----" << population[i].error_total << std::endl;
//        }
    }
}

#endif



void InitGene()
{
	unsigned int base = 1;

	for(int i = 0;i < (GENE_LENGTH / PIECE_NUM);++i){
		base *= 2;
	}

	srand(time(NULL));
	for(int i = 0;i < MAX;++i){
		unsigned int temp = 0;
		for(int j = 0;j < PIECE_NUM; ++j){
			unsigned int r = rand(base + 1);
			r <<= (GENE_LENGTH / PIECE_NUM) * j;
			temp |= r;
		}
		population[i].DNA = temp;
		population[i].pid_run = 0;          //設所有母體基因為｢尚未運算｣
	}
}


void FitnessFunc()              //計算Fitness，愈大愈好
{
    int max = 0;                //目前Fitness最大值
    int best_gene_position = 0; //最好的基因位置
    int error = 0;              //PID回傳結果
    int Magnification = 100;    //放大倍率

    for(int i = 0;i < MAX; i++) {
        if(population[i].pid_run == 0){         //跳過母體基因為｢已經運算｣
           error = PID(&population[i]);
           if(error != 0){                      //取誤差值倒數
                population[i].error_total = (unsigned long)Magnification / error;
           }

           population[i].pid_run = 1;           //計算完，設為｢已經運算｣
           calculation_num++;
        }
        if(max < population[i].error_total) {   //找最佳基因
            max = population[i].error_total;
            best_gene_position = i;
        }
    }
    best_gene = population[best_gene_position]; //選出最佳基因
}


//a的第bit_no1位元，與b的第bit_no2位元交換
void SwapBit(Gene *a, Gene *b, int bit_no1, int bit_no2){
    int i, tempa = a->DNA, tempb = b->DNA;
    tempa >>= GENE_LENGTH - bit_no1;
    tempb >>= GENE_LENGTH - bit_no2;
    if((tempa &= 0x1) != (tempb & 0x1)){
        int temp1 = 1, temp2 = 1;
        for(i = 0; i < GENE_LENGTH - bit_no1; i++){temp1 *= 2;}
        for(i = 0; i < GENE_LENGTH - bit_no2; i++){temp2 *= 2;}
        if(tempa == 0){
            a->DNA += temp1; b->DNA -= temp2;
        }else{
            a->DNA -= temp1; b->DNA += temp2;
        }
    }
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

            SwapBit(&population[counter], &population[counter+1], point_switch1, point_switch1); //二點交配
            SwapBit(&population[counter], &population[counter+1], point_switch2, point_switch2);

            population[counter].pid_run = 0;  //設所有新生子代，pid_run為｢尚未運算｣
            population[counter+1].pid_run = 0;

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
        for(i = 0; i < GENE_LENGTH - random_bit; i++){factor *= 2;}
        if(temp == 0){
            population[random_gene].DNA += factor;
        }else{
            population[random_gene].DNA -= factor;
        }
        population[random_gene].pid_run = 0;  //經過突變，pid_run設為｢尚未運算｣

        counter++;
	}

    memcpy(&population[0], &best_gene, sizeof(Gene)); //保留最佳母體
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
int PID(Gene *gene)
{	
	double input = 0;			//電壓
	double pre_input	=	input;	//上次電壓
	double output = 0;			//輸出
	double error  = 0;			//錯誤值
	double integral = (gene->DNA&0x03e0>>5) * error;
	double diff_input = input - pre_input;
	int count = 0;

	//輸入
	input   =   INIT_V;
	//迴圈
	while(count < 100)	{
		error = neural_network_calculate((int)round(input)) - setpoint;
		gene->error_total += error;
		//更新
		pre_input = input;
		input = (gene->DNA&0x7c00>>10) + integral - ((gene.DNA&0x001f)*diff_input);
		integral = (gene->DNA&0x03e0>>5)*error;
		diff_input = input - pre_input;
		count++;
	}
	return (int)round(error);
	
}
int neural_network_calculate(int input)
{
	double scaled_input = 0;
	double result = 0;
	double node[NEURON_NUM] = {0}; //隱藏層神經元節點
	int output = 0;

	scaled_input = 2*(input-1)/(256-1)-1;

    node[0] = tanh(-256.74-118.21*scaled_input);
    node[1] = tanh(-878.608+283.964*scaled_input);
    node[2] = tanh(246.978+35.8896*scaled_input);
    node[3] = tanh(336.655+94.8208*scaled_input);
    node[4] = tanh(-76.068-20.5661*scaled_input);
    node[5] = tanh(9.73519+6.46243*scaled_input);
    node[6] = tanh(0.657564+0.0919162*scaled_input);
    node[7] = tanh(1.38762-0.573859*scaled_input);

    result = 594.589-503.573*node[0]+378.605*node[1]-221.175*node[2]+435.93*node[3]-233.65*node[4]-524.598*node[5]-534.361*node[6]-68.8514*node[7];
	output = (int)round(result); //結果四捨五入
	return output;
}
/**************

放到Arduino的AVR Compiler記得砍掉main

**************/

int main(void)
{
    std::cout<<"set desired value you want:";
    std::cin>>setpoint;
//	GeneticAlgorithm();
	FitnessFunc();

//	for(int i = 0;i < MAX;++i){
//		int temp = population[i].DNA;
//		for(int j = 0;j < GENE_LENGTH;++j){
//			std::cout << (temp & 0x1);
//			temp >>= 1;
//		}
//		std::cout << std::endl;
//	}


//    ForVerify_Main();     //測試聯立方程式

    InitGene();
    //ForTestFitness();
    if(current_itera==1){
        P = best_gene.DNA&0x7c00>>10;
        I = best_gene.DNA&0x03e0>>5;
        D = best_gene.DNA&0x001f;
        std::cout << "--------------1st------" << "best Fitness:"
            << best_gene.error_total << "-------" << std::endl
            << P << " "
            << I << " "
            << D << std::endl<<std::endl;
//        for(int i = 0; i < MAX; i++){
//            showDNA(population[i].DNA);
//            std::cout << "-----" << population[i].error_total << std::endl;
//        }
    }

    while(current_itera<ITERA){
        Crossover();
        Mutation();
        //ForTestFitness();
        P = best_gene.DNA&0x7c00>>10;
        I = best_gene.DNA&0x03e0>>5;
        D = best_gene.DNA&0x001f;
        std::cout << "-------" << current_itera << "nd----best Fitness:"
            << best_gene.error_total << "-------" << std::endl
            << P << " "
            << I << " "
            << D << std::endl<<std::endl;
//        for(int i = 0; i < MAX; i++){
//            showDNA(population[i].DNA);
//            std::cout << "-----" << population[i].error_total << std::endl;
//        }
        FitnessFunc();

    }

    std::cout << "Total Calculation Num: " << calculation_num
        << "  (Original: " << MAX*ITERA << ")" << std::endl;
	getchar();
	return 0;
}
