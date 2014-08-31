#
# league.rb サッカーリーグ戦の結果を集計する
#
$KCODE = 'SJIS'

$col_home_team = 0 # ホームチーム名のカラム
$col_away_team = 3 # アウェーチーム名のカラム
$col_home_goal = 1 # ホームチーム得点のカラム
$col_away_goal = 2 # アウェーチーム得点のカラム

$pts_win = 3 # 勝ち試合の勝ち点
$pts_dra = 1 # 分け試合の勝ち点
$pts_los = 0 # 負け試合の勝ち点


def input_data(dat_file)
	game_ary = Array.new

	begin
		open(dat_file) do |f|
			while line = f.gets
				if /^#/ !~ line && line.size > 1
					game = line.chomp.split(/[\t-]+/)
					game_ary << {
						'home_team'=>game[$col_home_team],
						'away_team'=>game[$col_away_team],
						'home_goal'=>game[$col_home_goal].to_i,
						'away_goal'=>game[$col_away_goal].to_i
					}
				end
			end
		end
	rescue
		abort("Unable to open \"#{dat_file}\".\n")
	end

	return game_ary
end


def search_team(league_table, team)
	return nil if (league_table.size == 0)

	(0...league_table.size).each do |i|
		if (league_table[i]['name'] == team)
			return i
		end
	end

	return nil
end


def compute_score(team)
	team['goal_dif'] = team['goal_for'] - team['goal_aga']
	if team['goal_dif'] > 0
		team['pts'] = $pts_win
		team['win'] = 1
		team['dra'] = 0
		team['los'] = 0
	elsif team['goal_dif'] == 0
		team['pts'] = $pts_dra
		team['win'] = 0
		team['dra'] = 1
		team['los'] = 0
	else
		team['pts'] = $pts_los
		team['win'] = 0
		team['dra'] = 0
		team['los'] = 1
	end
	team['num'] = 1
end


def set_home_team(game)
	home = {
		'name'=>game['home_team'],
		'goal_for'=>game['home_goal'],
		'goal_aga'=>game['away_goal']
	}
	compute_score(home)

	return home
end


def set_away_team(game)
	away = {
		'name'=>game['away_team'],
		'goal_for'=>game['away_goal'],
		'goal_aga'=>game['home_goal']
	}
	compute_score(away)

	return away
end


def add_team_result(league_table, i, team)
	league_table[i]['num'] += team['num']
	league_table[i]['pts'] += team['pts']
	league_table[i]['win'] += team['win']
	league_table[i]['dra'] += team['dra']
	league_table[i]['los'] += team['los']
	league_table[i]['goal_for'] += team['goal_for']
	league_table[i]['goal_aga'] += team['goal_aga']
	league_table[i]['goal_dif'] += team['goal_dif']
end


def set_league_table(game_ary)
	league_table = Array.new

	game_ary.each do |game|
		team_home = set_home_team(game)
		team_away = set_away_team(game)

		if ((i = search_team(league_table, team_home['name'])) == nil)
			league_table << team_home
		else
			add_team_result(league_table, i, team_home)
		end

		if ((i = search_team(league_table, team_away['name'])) == nil)
			league_table << team_away
		else
			add_team_result(league_table, i, team_away)
		end
	end

	# 安定ソートで被ゴール数→ゴール数→得失点差→勝ち点の順で整列すると順位で並ぶ
	# リーグ戦の規程によってはこのやり方ではダメ
	i = 0
	league_table = league_table.sort_by do |a|
		[a['goal_aga'], i -= 1]	
	end
	league_table = league_table.sort_by do |a|
		[a['goal_for'], i += 1]	
	end
	league_table = league_table.sort_by do |a|
		[a['goal_dif'], i += 1]	
	end
	league_table = league_table.sort_by do |a|
		[a['pts'], i += 1]	
	end
	league_table.reverse!

	return league_table
end


def print_league_table(league_table)
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
	)

	league_table.each_with_index do |t, i|
		printf("%.6s\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\n",
			t['name'], t['num'], t['win'], t['dra'], t['los'], t['goal_for'], t['goal_aga'], t['goal_dif'], t['pts']
		)
	end
end


if (__FILE__ == $0)
	dat_file = ARGV.shift
	abort unless dat_file

	game_ary = input_data(dat_file)
	league_table = set_league_table(game_ary)

	print_league_table(league_table)

#	warn('Hit return key.')
#	STDIN.getc()
end

