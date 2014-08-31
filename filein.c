#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "league.h"
#include "sort.h"
#include "list.h"


struct list *search_team(struct list *head, char *team)
{
	struct list *p;

	for (p = head->next; p != head; p = p->next) {
		if (strncmp(((TEAM *) p->data)->name, team, strlen(team)) == 0) {
			return p;
		}
	}

	return NULL;
}


void compute_score(TEAM *team)
{
	team->gdi = team->gfo - team->gag;
	if (team->gfo > team->gag) {
		team->pts = PTS_WIN;
		team->win = 1;
		team->dra = team->los = 0;
	}
	else if (team->gfo == team->gag) {
		team->pts = PTS_DRA;
		team->dra = 1;
		team->win = team->los = 0;
	}
	else {
		team->pts = PTS_LOS;
		team->los = 1;
		team->win = team->dra = 0;
	}
	team->num = 1;
}


void set_home_team(TEAM *team, char **token)
{
	strcpy(team->name, token[HOME_TEAM_COL]);
	team->gfo = atoi(token[HOME_GOAL_COL]);
	team->gag = atoi(token[AWAY_GOAL_COL]);
	compute_score(team);
}


void set_away_team(TEAM *team, char **token)
{
	strcpy(team->name, token[AWAY_TEAM_COL]);
	team->gfo = atoi(token[AWAY_GOAL_COL]);
	team->gag = atoi(token[HOME_GOAL_COL]);
	compute_score(team);
}


void add_team_result(struct list *p, TEAM team)
{
	((TEAM *) p->data)->num += team.num;
	((TEAM *) p->data)->pts += team.pts;
	((TEAM *) p->data)->win += team.win;
	((TEAM *) p->data)->dra += team.dra;
	((TEAM *) p->data)->los += team.los;
	((TEAM *) p->data)->gfo += team.gfo;
	((TEAM *) p->data)->gag += team.gag;
	((TEAM *) p->data)->gdi += team.gdi;
}


int cmp_gag(const void *t1, const void *t2)
{
	return (((TEAM *) t1)->gag - ((TEAM *) t2)->gag);
}


int cmp_gfo(const void *t1, const void *t2)
{
	return (((TEAM *) t2)->gfo - ((TEAM *) t1)->gfo);
}


int cmp_gdi(const void *t1, const void *t2)
{
	return (((TEAM *) t2)->gdi - ((TEAM *) t1)->gdi);
}


int cmp_pts(const void *t1, const void *t2)
{
	return (((TEAM *) t2)->pts - ((TEAM *) t1)->pts);
}


TEAM *input_dat(FILE *fp, int *num_team)
{
	char line[SIZE_STR], **token;
	int i, num_token;
	int cmp_pts();
	TEAM team_home, team_away, *team_ary;
	struct list *team_list, *p;

	*num_team = 0;
	team_list = init_list();
	while (fgets(line, SIZE_STR, fp) != NULL) {
		if (line[0] != '#') {
			token = tokenize_str(line, "-\t\n", &num_token);
			if (num_token >= 4) {
				set_home_team(&team_home, token);
				set_away_team(&team_away, token);

				if ((p = search_team(team_list, team_home.name)) == NULL) {
					push_queue(team_list, &team_home, sizeof(team_home));
					(*num_team)++;
				}
				else {
					add_team_result(p, team_home);
				}

				if ((p = search_team(team_list, team_away.name)) == NULL) {
					push_queue(team_list, &team_away, sizeof(team_away));
					(*num_team)++;
				}
				else {
					add_team_result(p, team_away);
				}
			}
			free_token_ary(token, num_token);
		}
	}

	team_ary = (TEAM *) malloc(sizeof(TEAM) * (*num_team));
	if (team_ary == NULL) {
		fprintf(stderr, "ERROR: Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < *num_team; i++) {
		pop_queue(team_list, &team_ary[i], sizeof(TEAM));
	}
	delete_list(team_list);

	/* 安定ソートで被ゴール数→ゴール数→得失点差→勝ち点の順で整列すると順位で並ぶ */
	/* リーグ戦の規程によってはこのやり方ではダメ */
	bubblesort(team_ary, *num_team, sizeof(TEAM), cmp_gag);
	bubblesort(team_ary, *num_team, sizeof(TEAM), cmp_gfo);
	bubblesort(team_ary, *num_team, sizeof(TEAM), cmp_gdi);
	bubblesort(team_ary, *num_team, sizeof(TEAM), cmp_pts);

	return team_ary;
}

