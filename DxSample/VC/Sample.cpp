#include "DxLib.h"

#define BLOCK_NUM 12
#define BLOCK_NUM2 4
#define WINDOW_X 640
#define WINDOW_Y 600

class EnemyBlock {
public:

	int x, y;
	int width, height;
	int BarGraph;
	bool LiveFlag;

	void Draw() {
		//もし存在フラグをオンだったら敵ブロックを描画する
		if (LiveFlag) {
			DrawGraph(x, y, BarGraph, TRUE);
		}
	}

	EnemyBlock(int setX, int setY) {
		x = setX;
		y = setY;
		//敵ブロックのグラフィックをメモリにロード
		BarGraph = LoadGraph("bronsbar.png");
		// 敵ブロックのグラフィックのサイズをえる
		GetGraphSize(BarGraph, &width, &height);
		LiveFlag = true;
	}

	void All() {
		//描画処理
		Draw();
	}
};

class MyBlock {
public:

	int x, y;
	int width, height;
	int BarGraph;
	bool LiveFlag;

	void Draw() {
		//もし存在フラグをオンだったら自分のブロックを描画する
		if (LiveFlag) {
			DrawGraph(x, y, BarGraph, TRUE);
		}
	}

	MyBlock(int setX, int setY) {
		x = setX;
		y = setY;
		//ブロックのグラフィックをメモリにロード
		BarGraph = LoadGraph("mybar.png");
		// ブロックのグラフィックのサイズをえる
		GetGraphSize(BarGraph, &width, &height);
		LiveFlag = true;
	}

	void All() {
		//描画処理
		Draw();
	}
};

class Bar {
public:
	int x, y, width, height;
	int speed;
	int BarGraph;

	Bar() {
		//プレイヤーの初期位置
		x = 320;
		y = 400;
		//プレイヤーを動かす速さ
		speed = 10;
		//プレイヤーのグラフィックをメモリにロード
		BarGraph = LoadGraph("bar.png");
		//プレイヤーのグラフィックサイズを得る
		GetGraphSize(BarGraph, &width, &height);
		//width = 80
	}

	void Move() {
		// →キーを押していたらプレイヤーを右に移動させる
		if (CheckHitKey(KEY_INPUT_RIGHT) && x < 640) {
			x += speed;
		}
		// ←キーを押していたらプレイヤーを左に移動させる
		if (CheckHitKey(KEY_INPUT_LEFT) && x > 0) {
			x -= speed;
		}
		// プレイヤーが画面左端からはみ出そうになっていたら画面内の座標に戻してあげる
		if (x < 0) x = 0;

		//　プレイヤーが画面右端からはみ出そうになっていたら画面内の座標に戻してあげる
		if (x > 640 - 80) x = 640 - 80;

	}

	void Draw() {
		//プレイヤーの描画
		DrawExtendGraph(x, y, x + width, y + height, BarGraph, TRUE);
	}

	void All() {
		//プレイヤーの移動制御
		Move();
		//描画処理
		Draw();
	}

};

class Ball {
public:
	int x, y;
	int width, height;
	int dx, dy;
	int speed;
	int soundflag;
	int BallGraph;

	Ball() {
		//ボールの初期位置
		x = 320;
		y = 300;
		//xとyの移動量
		dx = 0;
		dy = 0;
		//ボールの速さ
		speed = 5;
		//ボールのグラフィックをメモリにロード
		BallGraph = LoadGraph("dango.png");
		//ボールのグラフィックサイズを得る
		GetGraphSize(BallGraph, &width, &height);
	}

	void Move() {
		//移動量加算
		x += speed * dx;
		y += speed * dy;

	}

	void Draw() {
		//ボールの描画
		DrawExtendGraph(x, y, x + width, y + height, BallGraph, TRUE);
	}

	void All() {
		//ボールの移動制御
		Move();
		//描画処理
		Draw();
	}
};

class GameControl {
public:
	//EnemyBlockクラスのポインタ
	EnemyBlock* bl[BLOCK_NUM];
	//MyBlockクラスのポインタ
	MyBlock* bl2[BLOCK_NUM2];
	//Barクラスのポインタ
	Bar* BarPos;
	//Ballクラスのポインタ
	Ball* BallPos;

	int life;
	int stage;
	int ZankiGraph;
	int TitleGraph, TitleGraph2;
	int GameoverGraph;
	int ClearGraph;
	bool pushFlag;
	bool boundflag, eblockflag, myblockflag, failflag;
	bool gameflag, clearflag;
	int dsound, ebsound, mbsound, failsound;
	int gamesound, gameclearsound;

	//スペースキーの長押しによる連続true判定を防ぐメソッド
	bool PushSpace() {
		
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			if (!pushFlag) {
				pushFlag = true;
				return true;
			}
		}
		else {
			pushFlag = false;
		}
		return false;
	}

	GameControl() {
		//プレイヤーとボールのインスタンス生成
		BarPos = new Bar();
		BallPos = new Ball();
		//残機の数
		life = 2;
		//条件分岐のための関数
		stage = 0;
		//タイトル、ゲームオーバー、ゲームクリア画面のファイル読み込み
		ZankiGraph = LoadGraph("zanki.png");
		TitleGraph = LoadGraph("title.png");
		TitleGraph2 = LoadGraph("2title.png");
		GameoverGraph = LoadGraph("gameover.png");
		ClearGraph = LoadGraph("gameclear.png");
		failsound = LoadSoundMem("sippai.mp3");
		ebsound = LoadSoundMem("bound.mp3");
		dsound = LoadSoundMem("destroy.mp3");
		mbsound = LoadSoundMem("mbdestroy.mp3");
		gamesound = LoadSoundMem("sanjinooyatsu.mp3");
		gameclearsound = LoadSoundMem("trumpet1.mp3");


		pushFlag = false;

			//ブロックの配置
		for (int i = 0; i < BLOCK_NUM; i++) {
			if (i < 4) {
				bl[i] = new EnemyBlock( 30 + (150) * i, 50 * 1);
			}
			else if (i > 3 && i < 8) {
				bl[i] = new EnemyBlock( 100 + (150) * (i-4), 50 * 2);
			}
			else {
				bl[i] = new EnemyBlock( 30 + (150) * (i-8), 50 * 3);
			}
		}
		for (int i = 0; i < BLOCK_NUM2; i++) {
			bl2[i] = new MyBlock(40 + (i % 4) * 150, 560 + (i / 4) * 40);
		}
		
	}

	//コンストラクタで実体化させたものをすべてdelete
	~GameControl() {
		for (int i = 0; i < BLOCK_NUM; i++) {
			delete bl[i];
		}
		for (int i = 0; i < BLOCK_NUM2; i++) {
			delete bl2[i];
		}
		delete BarPos;
		delete BallPos;
	}	

	void Title() {
		//タイトル画面の描画
		DrawGraph(0, 0, TitleGraph, TRUE);
		if (PushSpace()) {
			stage = 1;
			
		}
	}
	void Title2() {
		DrawGraph(0, 0, TitleGraph2, TRUE);
		if (PushSpace()) {
			//BGMをループで再生
			PlaySoundMem(gamesound, DX_PLAYTYPE_LOOP, TRUE);
			//Ballの位置を初期位置に戻す
			BallPos->x = 320;
			BallPos->y = 300;
			stage = 2;
		}
	}

	void Game() {
		
		//ブロックそれぞれの処理
		for (int i = 0; i < BLOCK_NUM; i++) {
			bl[i]->All();
		}
		for (int j = 0; j < BLOCK_NUM2; j++) {
			bl2[j]->All();
		}
		//プレイヤーの処理
		BarPos->All();
		//ボールの動き
	    BallPos->All();

		//残機表示
		for (int i = 0; i < life; i++) {
			//文字の描画
			DrawFormatString(10, 5, GetColor(255, 255, 0), "残機");
			//残機の描画
			DrawGraph(20+i*30 , 25, ZankiGraph, TRUE);
		}

		//ゲームオーバー判定
		for (int i = 0; i < BLOCK_NUM; i++) {
			//もし残っている相手のブロックが存在していたらbraek
			if (bl[i]->LiveFlag)break;
			//すべての相手のブロックを壊していた場合の処理
			if (i==BLOCK_NUM - 1) {
				//ボールを止める
				BallPos->dx = 0;
				BallPos->dy = 0;
				stage = 3;
				//BGMを止める
				StopSoundMem(gamesound);

			}
		}
		for (int j = 0; j < BLOCK_NUM2; j++) {
			//もし残っている自分のブロックが存在していたらbraek
			if (bl2[j]->LiveFlag)break;
			//すべてのブロックを壊していた場合の処理
			if (j==BLOCK_NUM2 - 1) {
				//ボールを止める
				BallPos->dx = 0;
				BallPos->dy = 0;
				stage = 4;
				//BGMを止める
				StopSoundMem(gamesound);
				clearflag = true;
				if (clearflag) {
					//クリアした時の音の出力
					PlaySoundMem(gameclearsound, DX_PLAYTYPE_BACK);
					clearflag = false;
				}
			}
			
		}

		//ボールが動いていて残機が残っている場合に処理
		if (BallPos->dx != 0 && BallPos->dy !=0 && life > 0) {
			//ボールが右の壁に衝突したら、ボールのx方向の向きを反転させて跳ね返す
			if (BallPos->x > 640) {
				BallPos->dx = -1;
				//衝突音フラグをオンにする
				boundflag = true;
			}
			//ボールが左の壁に衝突したら、ボールのx方向の向きを反転させて跳ね返す
			if (BallPos->x < 0) {
				BallPos->dx = 1;
				//衝突音フラグをオンにする
				boundflag = true;
			}
			//ボールが上の壁に衝突したら、ボールのY方向の向きは反転させて跳ね返す
			if (BallPos->y < 0) {
				BallPos->dy = 1;
				//衝突音フラグをオンにする
				boundflag = true;
			}
			//プレイヤーとボールが接触したら、ボールのY方向の向きを反転させて跳ね返す
			if (BallPos->x > BarPos->x - BarPos->width/2 && BallPos->x < BarPos->x + BarPos->width && BallPos->y + BallPos->height/2 > BarPos->y && 
				BallPos->y + BallPos->height / 2 < BarPos->y+BarPos->height)   {
				BallPos->dy = -1;
				//衝突音フラグをオンにする
				boundflag = true;
			}
		
			//ボールが下からプレイヤーに接触したら、ボールのY方向の向きを反転させて跳ね返す
			if (BallPos->x > BarPos->x - BarPos->width/2 && BallPos->x < BarPos->x + BarPos->width && BallPos->y - BallPos->height/2 < BarPos->y &&
				BallPos->y - BallPos->height / 2 > BarPos->y - BarPos->height) {
				BallPos->dy = 1;
				//衝突音フラグをオンにする
				boundflag = true;
			}
			//画面下にボールが落ちた場合の処理
			if (BallPos->y > 600) {
				//ボールの初期位置
				BallPos->x = 320;
				BallPos->y = 300;
				//ボールの移動量
				BallPos->dx = 0;
				BallPos->dy = 0;
				//残機を1減らす
				life--; 
				//失敗音フラグをオンにする
				failflag = true;
			}
			//敵ブロックとの当たり判定
			for (int i = 0; i < BLOCK_NUM; i++) {
				//敵ブロックが残っている場合処理
				if (bl[i]->LiveFlag) {
					//敵ブロックの上側の当たり判定
					if (BallPos->x > bl[i]->x&& BallPos->x < bl[i]->x + bl[i]->width &&
						BallPos->y + BallPos->height/2 > bl[i]->y&& BallPos->y + BallPos->height/2 < bl[i]->y + bl[i]->height) {
						//敵ブロックの存在フラグをオフにする
						bl[i]->LiveFlag = false;
						//敵ブロックの破壊音フラグをオンにする
						eblockflag = true;
						//y方向の運動量を反転させる
						BallPos->dy *= -1;
					}
					//敵ブロックの下側の当たり判定
					if (BallPos->x > bl[i]->x&& BallPos->x < bl[i]->x + bl[i]->width &&
						BallPos->y - BallPos->height/2 > bl[i]->y&& BallPos->y - BallPos->height/2 < bl[i]->y + bl[i]->height) {
						//敵ブロックの存在フラグをオフにする
						bl[i]->LiveFlag = false;
						//敵ブロックの破壊音フラグをオンにする
						eblockflag = true;
						//y方向の運動量を反転させる
						BallPos->dy *= -1;
					}
					//敵ブロックの左側の当たり判定
					if (BallPos->x + BallPos->height/2 > bl[i]->x&& BallPos->x + BallPos->height/2 < bl[i]->x + bl[i]->width &&
						BallPos->y > bl[i]->y && BallPos->y < bl[i]->y + bl[i]->height) {
						//敵ブロックの存在フラグをオフにする
						bl[i]->LiveFlag = false;
						//敵ブロックの破壊音フラグをオンにする
						eblockflag = true;
						//x方向の運動量を反転させる
						BallPos->dx *= -1;
					}
					//敵ブロックの右側の当たり判定
					if (BallPos->x - BallPos->height/2 > bl[i]->x&& BallPos->x - BallPos->height/2 < bl[i]->x + bl[i]->width &&
						BallPos->y > bl[i]->y && BallPos->y < bl[i]->y + bl[i]->height) {
						//敵ブロックの存在フラグをオフにする
						bl[i]->LiveFlag = false;
						//敵ブロックの破壊音フラグをオンにする
						eblockflag = true;
						//x方向の運動量を反転させる
						BallPos->dx *= -1;
					}
				}
			}
			for (int i = 0; i < BLOCK_NUM2; i++) {
				if (bl2[i]->LiveFlag) {

					//自分のブロックの上側の当たり判定
					if (BallPos->x > bl2[i]->x&& BallPos->x < bl2[i]->x + bl2[i]->width &&
						BallPos->y + BallPos->height/2 > bl2[i]->y&& BallPos->y + BallPos->height/2 < bl2[i]->y + bl2[i]->height) {
						//自分のブロックの存在フラグをオフにする
						bl2[i]->LiveFlag = false;
						//自分のブロックの破壊音フラグをオンにする
						myblockflag = true;
						//y方向の運動量を反転させる
						BallPos->dy *= -1;
					}
					//自分のブロックの下側の当たり判定
					if (BallPos->x > bl2[i]->x&& BallPos->x < bl2[i]->x + bl2[i]->width &&
						BallPos->y - BallPos->height/2 > bl2[i]->y&& BallPos->y - BallPos->height/2 < bl2[i]->y + bl2[i]->height) {
						//自分のブロックの存在フラグをオフにする
						bl2[i]->LiveFlag = false;
						//自分のブロックの破壊音フラグをオンにする
						myblockflag = true;
						//y方向の運動量を反転させる
						BallPos->dy *= -1;
					}
					//自分のブロックの左側の当たり判定
					if (BallPos->x + BallPos->height/2 > bl2[i]->x&& BallPos->x + BallPos->height/2 < bl2[i]->x + bl2[i]->width &&
						BallPos->y > bl2[i]->y && BallPos->y < bl2[i]->y + bl2[i]->height) {
						//自分のブロックの存在フラグをオフにする
						bl2[i]->LiveFlag = false;
						//自分のブロックの破壊音フラグをオンにする
						myblockflag = true;
						//x方向の運動量を反転させる
						BallPos->dx *= -1;
					}
					//自分のブロックの右側の当たり判定
					if (BallPos->x - BallPos->height/2 > bl2[i]->x&& BallPos->x - BallPos->height/2 < bl2[i]->x + bl2[i]->width &&
						BallPos->y > bl2[i]->y && BallPos->y < bl2[i]->y + bl2[i]->height) {
						//自分のブロックの存在フラグをオフにする
						bl2[i]->LiveFlag = false;
						//自分のブロックの破壊音フラグをオンにする
						myblockflag = true;
						//x方向の運動量を反転させる
						BallPos->dx *= -1;
					}
				}
			}
			//衝突音フラグがオンの時処理
			if (boundflag) {
				//衝突音を鳴らす
				PlaySoundMem(ebsound, DX_PLAYTYPE_BACK);
				//フラグをオフにする
				boundflag = false;
			}
			//敵ブロックの破壊音フラグをオンの時処理
			if (eblockflag) {
				//敵ブロックの破壊音を鳴らす
				PlaySoundMem(dsound, DX_PLAYTYPE_BACK);
				//フラグをオフにする
				eblockflag = false;
			}
			//自分のブロックの破壊音フラグをオンの時処理
			if (myblockflag) {
				//自分のブロックの破壊音を鳴らす
				PlaySoundMem(mbsound, DX_PLAYTYPE_BACK);
				//フラグをオフにする
				myblockflag = false;
			}
			//失敗音フラグがオンの時処理
			if (failflag) {
				//失敗音を鳴らす
				PlaySoundMem(failsound, DX_PLAYTYPE_BACK);
				//フラグをオフにする
				failflag = false;
			}
		}
		//残機が残っている場合の処理
		else if (life > 0) {
		    //文字の描画
			DrawFormatString(260, 330, GetColor(255, 255, 0), "スペースキーを押してね");
			//スペースキーが押されたらボールが動き出す
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				BallPos->dx = 1;
				BallPos->dy = 1;
			}
		}
		//残機がないか相手のブロックをすべて破壊してしまう
		else {
			stage = 3;
			//BGMを止める
			StopSoundMem(gamesound);
		}
			
	}


	
		
	
	void GameOver() {
		//ゲームオーバー画面の描画
		DrawGraph(0, 0, GameoverGraph, TRUE);
		//スペースキーが押されたらもう一度タイトルに戻る
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			stage = 0;
			life = 2;
			//ブロックの存在フラグをオンにする
			for (int i = 0; i < BLOCK_NUM; i++) {
				bl[i]->LiveFlag = true;
			}
			for (int i = 0; i < BLOCK_NUM2; i++) {
				bl2[i]->LiveFlag = true;
			}
		}
	}

	void GameClear() {
		//クリア画面の描画
		DrawGraph(0, 0, ClearGraph, TRUE);
		
		//スペースキーが押されたらもう一度タイトルに戻る
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			stage = 0;
			life = 2;
			//ブロックの存在フラグをオンにする
			for (int i = 0; i < BLOCK_NUM; i++) {
				bl[i]->LiveFlag = true;
			}
			for (int i = 0; i < BLOCK_NUM2; i++) {
				bl2[i]->LiveFlag = true;
			}
		}
	}

	//処理の分岐
	void All() {
		if (stage == 0)Title();
		if (stage == 1)Title2();
		if (stage == 2)Game();
		if (stage == 3)GameOver();
		if (stage == 4)GameClear();
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	//ウィンドウサイズで起動
	ChangeWindowMode(TRUE);
	//画面モードの設定
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);
	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)return -1;
	//グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//GameControlクラスのポインタ宣言
	GameControl* gamecontrol = new GameControl();
	//メッセージ処理
	while (ProcessMessage() != -1) {
		//裏画面の内容を表画面にコピーする
		ScreenFlip();
		//裏画面をクリアする
		ClearDrawScreen();
		//All関数の呼び出し
		gamecontrol->All();

		// Windows 特有の面倒な処理をＤＸライブラリにやらせる
		// -1 が返ってきたらループを抜ける
		if (ProcessMessage() < 0) break;

		// もしＥＳＣキーが押されていたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
		
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	//実体化させたgamecontrolをdelete
	delete gamecontrol;

	// ソフトの終了
	return 0;
}