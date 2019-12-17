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
		//�������݃t���O���I����������G�u���b�N��`�悷��
		if (LiveFlag) {
			DrawGraph(x, y, BarGraph, TRUE);
		}
	}

	EnemyBlock(int setX, int setY) {
		x = setX;
		y = setY;
		//�G�u���b�N�̃O���t�B�b�N���������Ƀ��[�h
		BarGraph = LoadGraph("bronsbar.png");
		// �G�u���b�N�̃O���t�B�b�N�̃T�C�Y������
		GetGraphSize(BarGraph, &width, &height);
		LiveFlag = true;
	}

	void All() {
		//�`�揈��
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
		//�������݃t���O���I���������玩���̃u���b�N��`�悷��
		if (LiveFlag) {
			DrawGraph(x, y, BarGraph, TRUE);
		}
	}

	MyBlock(int setX, int setY) {
		x = setX;
		y = setY;
		//�u���b�N�̃O���t�B�b�N���������Ƀ��[�h
		BarGraph = LoadGraph("mybar.png");
		// �u���b�N�̃O���t�B�b�N�̃T�C�Y������
		GetGraphSize(BarGraph, &width, &height);
		LiveFlag = true;
	}

	void All() {
		//�`�揈��
		Draw();
	}
};

class Bar {
public:
	int x, y, width, height;
	int speed;
	int BarGraph;

	Bar() {
		//�v���C���[�̏����ʒu
		x = 320;
		y = 400;
		//�v���C���[�𓮂�������
		speed = 10;
		//�v���C���[�̃O���t�B�b�N���������Ƀ��[�h
		BarGraph = LoadGraph("bar.png");
		//�v���C���[�̃O���t�B�b�N�T�C�Y�𓾂�
		GetGraphSize(BarGraph, &width, &height);
		//width = 80
	}

	void Move() {
		// ���L�[�������Ă�����v���C���[���E�Ɉړ�������
		if (CheckHitKey(KEY_INPUT_RIGHT) && x < 640) {
			x += speed;
		}
		// ���L�[�������Ă�����v���C���[�����Ɉړ�������
		if (CheckHitKey(KEY_INPUT_LEFT) && x > 0) {
			x -= speed;
		}
		// �v���C���[����ʍ��[����͂ݏo�����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă�����
		if (x < 0) x = 0;

		//�@�v���C���[����ʉE�[����͂ݏo�����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă�����
		if (x > 640 - 80) x = 640 - 80;

	}

	void Draw() {
		//�v���C���[�̕`��
		DrawExtendGraph(x, y, x + width, y + height, BarGraph, TRUE);
	}

	void All() {
		//�v���C���[�̈ړ�����
		Move();
		//�`�揈��
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
		//�{�[���̏����ʒu
		x = 320;
		y = 300;
		//x��y�̈ړ���
		dx = 0;
		dy = 0;
		//�{�[���̑���
		speed = 5;
		//�{�[���̃O���t�B�b�N���������Ƀ��[�h
		BallGraph = LoadGraph("dango.png");
		//�{�[���̃O���t�B�b�N�T�C�Y�𓾂�
		GetGraphSize(BallGraph, &width, &height);
	}

	void Move() {
		//�ړ��ʉ��Z
		x += speed * dx;
		y += speed * dy;

	}

	void Draw() {
		//�{�[���̕`��
		DrawExtendGraph(x, y, x + width, y + height, BallGraph, TRUE);
	}

	void All() {
		//�{�[���̈ړ�����
		Move();
		//�`�揈��
		Draw();
	}
};

class GameControl {
public:
	//EnemyBlock�N���X�̃|�C���^
	EnemyBlock* bl[BLOCK_NUM];
	//MyBlock�N���X�̃|�C���^
	MyBlock* bl2[BLOCK_NUM2];
	//Bar�N���X�̃|�C���^
	Bar* BarPos;
	//Ball�N���X�̃|�C���^
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

	//�X�y�[�X�L�[�̒������ɂ��A��true�����h�����\�b�h
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
		//�v���C���[�ƃ{�[���̃C���X�^���X����
		BarPos = new Bar();
		BallPos = new Ball();
		//�c�@�̐�
		life = 2;
		//��������̂��߂̊֐�
		stage = 0;
		//�^�C�g���A�Q�[���I�[�o�[�A�Q�[���N���A��ʂ̃t�@�C���ǂݍ���
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

			//�u���b�N�̔z�u
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

	//�R���X�g���N�^�Ŏ��̉����������̂����ׂ�delete
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
		//�^�C�g����ʂ̕`��
		DrawGraph(0, 0, TitleGraph, TRUE);
		if (PushSpace()) {
			stage = 1;
			
		}
	}
	void Title2() {
		DrawGraph(0, 0, TitleGraph2, TRUE);
		if (PushSpace()) {
			//BGM�����[�v�ōĐ�
			PlaySoundMem(gamesound, DX_PLAYTYPE_LOOP, TRUE);
			//Ball�̈ʒu�������ʒu�ɖ߂�
			BallPos->x = 320;
			BallPos->y = 300;
			stage = 2;
		}
	}

	void Game() {
		
		//�u���b�N���ꂼ��̏���
		for (int i = 0; i < BLOCK_NUM; i++) {
			bl[i]->All();
		}
		for (int j = 0; j < BLOCK_NUM2; j++) {
			bl2[j]->All();
		}
		//�v���C���[�̏���
		BarPos->All();
		//�{�[���̓���
	    BallPos->All();

		//�c�@�\��
		for (int i = 0; i < life; i++) {
			//�����̕`��
			DrawFormatString(10, 5, GetColor(255, 255, 0), "�c�@");
			//�c�@�̕`��
			DrawGraph(20+i*30 , 25, ZankiGraph, TRUE);
		}

		//�Q�[���I�[�o�[����
		for (int i = 0; i < BLOCK_NUM; i++) {
			//�����c���Ă��鑊��̃u���b�N�����݂��Ă�����braek
			if (bl[i]->LiveFlag)break;
			//���ׂĂ̑���̃u���b�N���󂵂Ă����ꍇ�̏���
			if (i==BLOCK_NUM - 1) {
				//�{�[�����~�߂�
				BallPos->dx = 0;
				BallPos->dy = 0;
				stage = 3;
				//BGM���~�߂�
				StopSoundMem(gamesound);

			}
		}
		for (int j = 0; j < BLOCK_NUM2; j++) {
			//�����c���Ă��鎩���̃u���b�N�����݂��Ă�����braek
			if (bl2[j]->LiveFlag)break;
			//���ׂẴu���b�N���󂵂Ă����ꍇ�̏���
			if (j==BLOCK_NUM2 - 1) {
				//�{�[�����~�߂�
				BallPos->dx = 0;
				BallPos->dy = 0;
				stage = 4;
				//BGM���~�߂�
				StopSoundMem(gamesound);
				clearflag = true;
				if (clearflag) {
					//�N���A�������̉��̏o��
					PlaySoundMem(gameclearsound, DX_PLAYTYPE_BACK);
					clearflag = false;
				}
			}
			
		}

		//�{�[���������Ă��Ďc�@���c���Ă���ꍇ�ɏ���
		if (BallPos->dx != 0 && BallPos->dy !=0 && life > 0) {
			//�{�[�����E�̕ǂɏՓ˂�����A�{�[����x�����̌����𔽓]�����Ē��˕Ԃ�
			if (BallPos->x > 640) {
				BallPos->dx = -1;
				//�Փˉ��t���O���I���ɂ���
				boundflag = true;
			}
			//�{�[�������̕ǂɏՓ˂�����A�{�[����x�����̌����𔽓]�����Ē��˕Ԃ�
			if (BallPos->x < 0) {
				BallPos->dx = 1;
				//�Փˉ��t���O���I���ɂ���
				boundflag = true;
			}
			//�{�[������̕ǂɏՓ˂�����A�{�[����Y�����̌����͔��]�����Ē��˕Ԃ�
			if (BallPos->y < 0) {
				BallPos->dy = 1;
				//�Փˉ��t���O���I���ɂ���
				boundflag = true;
			}
			//�v���C���[�ƃ{�[�����ڐG������A�{�[����Y�����̌����𔽓]�����Ē��˕Ԃ�
			if (BallPos->x > BarPos->x - BarPos->width/2 && BallPos->x < BarPos->x + BarPos->width && BallPos->y + BallPos->height/2 > BarPos->y && 
				BallPos->y + BallPos->height / 2 < BarPos->y+BarPos->height)   {
				BallPos->dy = -1;
				//�Փˉ��t���O���I���ɂ���
				boundflag = true;
			}
		
			//�{�[����������v���C���[�ɐڐG������A�{�[����Y�����̌����𔽓]�����Ē��˕Ԃ�
			if (BallPos->x > BarPos->x - BarPos->width/2 && BallPos->x < BarPos->x + BarPos->width && BallPos->y - BallPos->height/2 < BarPos->y &&
				BallPos->y - BallPos->height / 2 > BarPos->y - BarPos->height) {
				BallPos->dy = 1;
				//�Փˉ��t���O���I���ɂ���
				boundflag = true;
			}
			//��ʉ��Ƀ{�[�����������ꍇ�̏���
			if (BallPos->y > 600) {
				//�{�[���̏����ʒu
				BallPos->x = 320;
				BallPos->y = 300;
				//�{�[���̈ړ���
				BallPos->dx = 0;
				BallPos->dy = 0;
				//�c�@��1���炷
				life--; 
				//���s���t���O���I���ɂ���
				failflag = true;
			}
			//�G�u���b�N�Ƃ̓����蔻��
			for (int i = 0; i < BLOCK_NUM; i++) {
				//�G�u���b�N���c���Ă���ꍇ����
				if (bl[i]->LiveFlag) {
					//�G�u���b�N�̏㑤�̓����蔻��
					if (BallPos->x > bl[i]->x&& BallPos->x < bl[i]->x + bl[i]->width &&
						BallPos->y + BallPos->height/2 > bl[i]->y&& BallPos->y + BallPos->height/2 < bl[i]->y + bl[i]->height) {
						//�G�u���b�N�̑��݃t���O���I�t�ɂ���
						bl[i]->LiveFlag = false;
						//�G�u���b�N�̔j�󉹃t���O���I���ɂ���
						eblockflag = true;
						//y�����̉^���ʂ𔽓]������
						BallPos->dy *= -1;
					}
					//�G�u���b�N�̉����̓����蔻��
					if (BallPos->x > bl[i]->x&& BallPos->x < bl[i]->x + bl[i]->width &&
						BallPos->y - BallPos->height/2 > bl[i]->y&& BallPos->y - BallPos->height/2 < bl[i]->y + bl[i]->height) {
						//�G�u���b�N�̑��݃t���O���I�t�ɂ���
						bl[i]->LiveFlag = false;
						//�G�u���b�N�̔j�󉹃t���O���I���ɂ���
						eblockflag = true;
						//y�����̉^���ʂ𔽓]������
						BallPos->dy *= -1;
					}
					//�G�u���b�N�̍����̓����蔻��
					if (BallPos->x + BallPos->height/2 > bl[i]->x&& BallPos->x + BallPos->height/2 < bl[i]->x + bl[i]->width &&
						BallPos->y > bl[i]->y && BallPos->y < bl[i]->y + bl[i]->height) {
						//�G�u���b�N�̑��݃t���O���I�t�ɂ���
						bl[i]->LiveFlag = false;
						//�G�u���b�N�̔j�󉹃t���O���I���ɂ���
						eblockflag = true;
						//x�����̉^���ʂ𔽓]������
						BallPos->dx *= -1;
					}
					//�G�u���b�N�̉E���̓����蔻��
					if (BallPos->x - BallPos->height/2 > bl[i]->x&& BallPos->x - BallPos->height/2 < bl[i]->x + bl[i]->width &&
						BallPos->y > bl[i]->y && BallPos->y < bl[i]->y + bl[i]->height) {
						//�G�u���b�N�̑��݃t���O���I�t�ɂ���
						bl[i]->LiveFlag = false;
						//�G�u���b�N�̔j�󉹃t���O���I���ɂ���
						eblockflag = true;
						//x�����̉^���ʂ𔽓]������
						BallPos->dx *= -1;
					}
				}
			}
			for (int i = 0; i < BLOCK_NUM2; i++) {
				if (bl2[i]->LiveFlag) {

					//�����̃u���b�N�̏㑤�̓����蔻��
					if (BallPos->x > bl2[i]->x&& BallPos->x < bl2[i]->x + bl2[i]->width &&
						BallPos->y + BallPos->height/2 > bl2[i]->y&& BallPos->y + BallPos->height/2 < bl2[i]->y + bl2[i]->height) {
						//�����̃u���b�N�̑��݃t���O���I�t�ɂ���
						bl2[i]->LiveFlag = false;
						//�����̃u���b�N�̔j�󉹃t���O���I���ɂ���
						myblockflag = true;
						//y�����̉^���ʂ𔽓]������
						BallPos->dy *= -1;
					}
					//�����̃u���b�N�̉����̓����蔻��
					if (BallPos->x > bl2[i]->x&& BallPos->x < bl2[i]->x + bl2[i]->width &&
						BallPos->y - BallPos->height/2 > bl2[i]->y&& BallPos->y - BallPos->height/2 < bl2[i]->y + bl2[i]->height) {
						//�����̃u���b�N�̑��݃t���O���I�t�ɂ���
						bl2[i]->LiveFlag = false;
						//�����̃u���b�N�̔j�󉹃t���O���I���ɂ���
						myblockflag = true;
						//y�����̉^���ʂ𔽓]������
						BallPos->dy *= -1;
					}
					//�����̃u���b�N�̍����̓����蔻��
					if (BallPos->x + BallPos->height/2 > bl2[i]->x&& BallPos->x + BallPos->height/2 < bl2[i]->x + bl2[i]->width &&
						BallPos->y > bl2[i]->y && BallPos->y < bl2[i]->y + bl2[i]->height) {
						//�����̃u���b�N�̑��݃t���O���I�t�ɂ���
						bl2[i]->LiveFlag = false;
						//�����̃u���b�N�̔j�󉹃t���O���I���ɂ���
						myblockflag = true;
						//x�����̉^���ʂ𔽓]������
						BallPos->dx *= -1;
					}
					//�����̃u���b�N�̉E���̓����蔻��
					if (BallPos->x - BallPos->height/2 > bl2[i]->x&& BallPos->x - BallPos->height/2 < bl2[i]->x + bl2[i]->width &&
						BallPos->y > bl2[i]->y && BallPos->y < bl2[i]->y + bl2[i]->height) {
						//�����̃u���b�N�̑��݃t���O���I�t�ɂ���
						bl2[i]->LiveFlag = false;
						//�����̃u���b�N�̔j�󉹃t���O���I���ɂ���
						myblockflag = true;
						//x�����̉^���ʂ𔽓]������
						BallPos->dx *= -1;
					}
				}
			}
			//�Փˉ��t���O���I���̎�����
			if (boundflag) {
				//�Փˉ���炷
				PlaySoundMem(ebsound, DX_PLAYTYPE_BACK);
				//�t���O���I�t�ɂ���
				boundflag = false;
			}
			//�G�u���b�N�̔j�󉹃t���O���I���̎�����
			if (eblockflag) {
				//�G�u���b�N�̔j�󉹂�炷
				PlaySoundMem(dsound, DX_PLAYTYPE_BACK);
				//�t���O���I�t�ɂ���
				eblockflag = false;
			}
			//�����̃u���b�N�̔j�󉹃t���O���I���̎�����
			if (myblockflag) {
				//�����̃u���b�N�̔j�󉹂�炷
				PlaySoundMem(mbsound, DX_PLAYTYPE_BACK);
				//�t���O���I�t�ɂ���
				myblockflag = false;
			}
			//���s���t���O���I���̎�����
			if (failflag) {
				//���s����炷
				PlaySoundMem(failsound, DX_PLAYTYPE_BACK);
				//�t���O���I�t�ɂ���
				failflag = false;
			}
		}
		//�c�@���c���Ă���ꍇ�̏���
		else if (life > 0) {
		    //�����̕`��
			DrawFormatString(260, 330, GetColor(255, 255, 0), "�X�y�[�X�L�[�������Ă�");
			//�X�y�[�X�L�[�������ꂽ��{�[���������o��
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				BallPos->dx = 1;
				BallPos->dy = 1;
			}
		}
		//�c�@���Ȃ�������̃u���b�N�����ׂĔj�󂵂Ă��܂�
		else {
			stage = 3;
			//BGM���~�߂�
			StopSoundMem(gamesound);
		}
			
	}


	
		
	
	void GameOver() {
		//�Q�[���I�[�o�[��ʂ̕`��
		DrawGraph(0, 0, GameoverGraph, TRUE);
		//�X�y�[�X�L�[�������ꂽ�������x�^�C�g���ɖ߂�
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			stage = 0;
			life = 2;
			//�u���b�N�̑��݃t���O���I���ɂ���
			for (int i = 0; i < BLOCK_NUM; i++) {
				bl[i]->LiveFlag = true;
			}
			for (int i = 0; i < BLOCK_NUM2; i++) {
				bl2[i]->LiveFlag = true;
			}
		}
	}

	void GameClear() {
		//�N���A��ʂ̕`��
		DrawGraph(0, 0, ClearGraph, TRUE);
		
		//�X�y�[�X�L�[�������ꂽ�������x�^�C�g���ɖ߂�
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			stage = 0;
			life = 2;
			//�u���b�N�̑��݃t���O���I���ɂ���
			for (int i = 0; i < BLOCK_NUM; i++) {
				bl[i]->LiveFlag = true;
			}
			for (int i = 0; i < BLOCK_NUM2; i++) {
				bl2[i]->LiveFlag = true;
			}
		}
	}

	//�����̕���
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
	//�E�B���h�E�T�C�Y�ŋN��
	ChangeWindowMode(TRUE);
	//��ʃ��[�h�̐ݒ�
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);
	//�c�w���C�u��������������
	if (DxLib_Init() == -1)return -1;
	//�O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	//GameControl�N���X�̃|�C���^�錾
	GameControl* gamecontrol = new GameControl();
	//���b�Z�[�W����
	while (ProcessMessage() != -1) {
		//����ʂ̓��e��\��ʂɃR�s�[����
		ScreenFlip();
		//����ʂ��N���A����
		ClearDrawScreen();
		//All�֐��̌Ăяo��
		gamecontrol->All();

		// Windows ���L�̖ʓ|�ȏ������c�w���C�u�����ɂ�点��
		// -1 ���Ԃ��Ă����烋�[�v�𔲂���
		if (ProcessMessage() < 0) break;

		// �����d�r�b�L�[��������Ă����烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
		
	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	//���̉�������gamecontrol��delete
	delete gamecontrol;

	// �\�t�g�̏I��
	return 0;
}