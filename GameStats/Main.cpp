# include <Siv3D.hpp> // OpenSiv3D v0.6.9

//todo:
// アシストフォント描画
// プレイヤーの数を変動できるようにする
// 枠線描画のプログラムブラッシュアップ
// UI更新
// 下スクロール上限設置
// 下スクロールしても項目が消えないようにする
// プログラムから名前変更できるようにする
// 人数をプログラムから変更できるようにする
void Main()
{
	//背景色
	Scene::SetBackground(Palette::Lightblue);

	// ウィンドウを 1280 * 720 にリサイズする
	Window::Resize(1280,720);

	// CSV ファイルからデータを読み込む
	CSV csv{ U"CSV/Game stats.csv" };

	if (not csv) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load `Game stats.csv`" };
	}

	//フォント群
	Font titleFont				{ 45,U"Font/jfdotfont-20150527/JF-Dot-ShinonomeMin12.ttf" };
	Font tableOfContentsFont	{ 30  ,U"Font/jfdotfont-20150527/JF-Dot-ShinonomeMin12.ttf"};
	Font playerFont				{ 30 };
	Font font{ 20 };

	//ドラッグした後の画面の補正座標
	double screenPos = 0;

	//プレイヤーの数
	int numberOfPlayers = 17;

	const int width = 7;//横幅
	const int height = numberOfPlayers + 1;//tableOfContents込みの縦幅
	const int startPos = 3;//tableOfContentsまでのセル数
	const Size cellSize = { 170,42 };//一セルのサイズ

	//CSVからの情報をしまう箱
	String title;
	Array<String> tableOfContents;

	//プレイヤーの名前とスコア群(一列ごと)
	Array<PlayerData> playerDatas;

	//タイトルにCSVから代入
	title = (String)csv[0][0];

	//PlayerDatasにCSVから代入
	for (int i = 0; i < numberOfPlayers; i++)
	{
		playerDatas << PlayerData{ csv[i + startPos]};
	}

	//最上位の項目をCSVから代入
	for (int y = 0; y < 1; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tableOfContents << csv[startPos - 1][x];
		}
	}

	while (System::Update())
	{
		//デバッグ用
		ClearPrint();

		//Sキーを入力したらセーブ
		if (KeyS.down())
		{
			//上書き、セーブするのはプレイヤーの名前とスコアのみ
			//縦を増加、PlayerDatasの分増加
			for (int y = 0; y < playerDatas.size(); y++)
			{
				//横を増加、項目の数分増加
				for (int x = 0 ; x < playerDatas[y].GetData().size(); x++)
				{
					//縦にずらす
					int csvY = y + startPos;
					csv[csvY][x] = playerDatas[y].GetData()[x];
				}
			}

			//更新
			csv.save(U"CSV/Game stats.csv");
		}

		//上下キーでスクロール
		if (KeyUp.pressed())screenPos += 3;
		if (KeyDown.pressed())screenPos -= 3;

		//マウスホイールでスクロール
		screenPos -= Mouse::Wheel() * 15;

		//画面外に出たら座標補正
		if (screenPos > 0)screenPos = 0;

		//セルの処理
		for (auto& pd : playerDatas)
		{
			pd.Update();
		}

		//描画
		//タイトルを描画
		titleFont(title).draw(5,screenPos,Palette::Black);

		//枠線を描画
		for (int i = 1; i < width + 1; i++)
		{
			//縦
			Line{ i * cellSize.x,cellSize.y * 2 + screenPos ,i * cellSize.x ,(cellSize.y * 2 + screenPos) + (cellSize.y * (numberOfPlayers + 1)) }.draw(4, Palette::Black);
		}

		for (int i = 0; i < height + 1; i++)
		{
			//横
			Line{ 0,(cellSize.y * 2 + screenPos)+ (cellSize.y * i) ,cellSize.x * width,(cellSize.y * 2 + screenPos) + (cellSize.y * i) }.draw(4, Palette::Black);
		}

		//枠線を描画
		for (int i = 1; i < width + 1; i++)
		{
			//縦
			Line{ i * cellSize.x,cellSize.y * 2 + screenPos ,i * cellSize.x ,(cellSize.y * 2 + screenPos) + (cellSize.y * (1)) }.draw(4, Palette::Yellow);
		}

		for (int i = 0; i < 2; i++)
		{
			//横
			Line{ 0,(cellSize.y * 2 + screenPos) + (cellSize.y * i) ,cellSize.x * width,(cellSize.y * 2 + screenPos) + (cellSize.y * i) }.draw(4, Palette::Yellow);
		}

		//表を描画
		for (int i = 0; i < numberOfPlayers; i++)
		{
			playerDatas[i].Draw(playerFont, (i * cellSize.y) + (cellSize.y * 3) + screenPos);
		}

		//最上位の項目を描画
		for (int i = 0; i < width; i++)
		{
			tableOfContentsFont(tableOfContents[i]).draw(i * cellSize.x + 10, cellSize.y * 2 + screenPos + 5, Palette::Red);
		}

		font(U"左クリックで増加、右クリックで減少、Sでセーブ").draw(0,cellSize.y * 1 + 5,Palette::Red);
	}
}

