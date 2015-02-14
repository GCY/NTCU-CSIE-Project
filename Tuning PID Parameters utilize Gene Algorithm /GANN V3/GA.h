#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

/***換到Arduino要把這區塊助解取消反之註解***/
/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#define time(value) analogRead(3)
#define srand(analog) randomSeed(analog)
#define rand(max) random(max+1)
#define NULL 0
*/

/***換到PC要把這區塊助解取消反之註解***/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#define rand(max) (rand() % max) + 1


#define MAX 30				//母體數量
#define ITERA 20			//繁衍世代
#define CROSSOVER_RATE 0.5 	//交配率
#define MUTATION_RATE 0.1 	//突變率
#define GENE_LENGTH 15		//基因長度(bit)
#define PIECE_NUM 3			//基因片段(P I D)
#define NEURON_NUM 8		// 神經元個數
#difine INIT_V 50		//初始電壓

typedef struct
{
	int DNA:GENE_LENGTH;		//基因長度15bit
	int select: 1;				//判斷此基因是否被選擇1bit
	int pid_run: 1;				//判斷此基因是否跑過PID 1bit
	unsigned long error_total;	//總累計誤差
}Gene;

extern Gene population[MAX];	//母代
extern Gene pool[MAX];			//交配池
extern Gene best_gene;			//選出最好的基因

extern void InitGene();			//完成
extern void Crossover();		//未完成 要從母代中選擇要交配的然後
extern void Mutation();			//未完成
extern void FitnessFunc();		//GCY
extern void GeneticAlgorithm();		//未完成


/**************

這個函數模擬PID跑一百次後得到總錯誤量，
寫交配跟突變函數的人要用到這個測試

傳入要跑的基因結構就可以了

**************/
extern int PID(Gene*);
extern int neural_network_calculate(int);

#endif
