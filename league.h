#define SIZE_STR 1024

#define PTS_WIN 3	/* 勝ちチームの勝ち点 */
#define PTS_DRA 1	/* 引き分けの勝ち点 */
#define PTS_LOS 0	/* 負けチームの勝ち点 */

#define HOME_TEAM_COL 0	/* ホームチーム名のカラム番号 */
#define AWAY_TEAM_COL 3	/* アウェイチーム名のカラム番号 */
#define HOME_GOAL_COL 1	/* ホームチームの得点のカラム番号 */
#define AWAY_GOAL_COL 2	/* アウェイチームの得点のカラム番号 */

typedef struct {
	char name[SIZE_STR];	/* チーム名 */
	int num;	/* 試合数 */
	int pts;	/* 勝ち点 */
	int win;	/* 勝ち数 */
	int dra;	/* 引き分け数 */
	int los;	/* 負け数 */
	int gfo;	/* ゴール数 */
	int gag;	/* 被ゴール数 */
	int gdi;	/* 得失点差 */
} TEAM;


/* 関数プロトタイプ宣言 */
void print_group(TEAM *, int);
void usage(void);

struct list *search_team(struct list *, char *);
void compute_score(TEAM *);
void set_home_team(TEAM *, char **);
void set_away_team(TEAM *, char **);
void add_team_result(struct list *, TEAM);
int cmp_pts(const void *, const void *);
TEAM *input_dat(FILE *, int *);


