# league サッカーリーグ戦集計ツール
サッカーのリーグ戦の勝ち点、得失点差などを集計するツールです。  
C 版と Ruby 版があり、どちらも同じ動作をします。


## 使い方
端末 (コマンドプロンプト) を開き、以下のようにコンパイル、実行して下さい。

    $ make
    $ ./league データファイル
    $ ruby league.rb データファイル


## データファイルの書式
データファイルはプレーンテキストで、一行に一試合ずつ記入します。

一行に最低限必要な項目は、「ホームチーム名」「ホームチーム得点」「アウェイチーム得点」「アウェイチーム名」です。
この 4 項目を、タブまたはハイフン (-) を区切り文字として、次の順序で並べて下さい。

    ホームチーム名[タブ]ホームチーム得点-アウェイチーム得点[タブ]アウェイチーム名[タブ]...

連続するタブとハイフンがあれば、まとめて一つの区切りとして扱われます。
タブとハイフンは区切り文字なので、チーム名には使えません。
5 番目以降の項目は無視されるので、メモなどを記入できます。
'#' で始まる行はコメントとみなされます。


    ### データファイルの例 - ワールドカップ 2014 グループリーグ C 組
    コロンビア	3	-	0	ギリシャ	2014-06-14
    コートジボワール	2	-	1	日本	2014-06-14
    
    コロンビア	2	-	1	コートジボワール	2014-06-19
    日本	0	-	0	ギリシャ	2014-06-19
    
    日本	1	-	4	コロンビア	2014-06-24
    ギリシャ	2	-	1	コートジボワール	2014-06-24