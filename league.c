/*
 * league.c サッカーリーグ戦の結果を集計する
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "league.h"


void print_group(TEAM *team, int num_team)
{
	int i;

//	printf("%.6s\t%.6s\t%.6s\t%.6s\t%.6s\t%.6s\t%.6s\t%.6s\t%.6s\n",
	printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		"チーム",
		"試合数",
		"勝ち数",
		"引分数",
		"負け数",
		"得　点",
		"失　点",
		"得失差",
		"勝ち点"
	);

	for (i = 0; i < num_team; i++) {
		printf("%.6s\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\n",
			team[i].name,
			team[i].num,
			team[i].win,
			team[i].dra,
			team[i].los,
			team[i].gfo,
			team[i].gag,
			team[i].gdi,
			team[i].pts
		);
	}
}


void usage(void)
{
	fprintf(stderr, "つかいかた: league データファイル\n");
	exit(EXIT_SUCCESS);
}


int main(int argc, char **argv)
{
	char dat_file[SIZE_STR];
	int num_team;
	FILE *fp;
	TEAM *team;

	if (argc == 2)
		strcpy(dat_file, argv[1]);
	else
		usage();

	fp = fopen(dat_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Unable to open \"%s\".\n", dat_file);
		return EXIT_FAILURE;
	}

	team = input_dat(fp, &num_team);
	print_group(team, num_team);

	free(team);
	fclose(fp);

	return EXIT_SUCCESS;
}
