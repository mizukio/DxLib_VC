//　 ここにサンプルプログラムをペーストしてください
// 初期状態ではサウンドノベル風文字列描画サンプルが入力されています。
// 　べつのサンプルプログラムを実行する場合は以下のプログラムをすべて
// 削除して、新たにコピー＆ペーストしてください。

// サウンドノベル風文字列描画、テキストバッファ使用バージョン
#include "DxLib.h"
#include <math.h>

// 文字のサイズ
#define MOJI_SIZE 24

// 仮想テキストバッファの横サイズ縦サイズ
#define STRBUF_WIDTH	24
#define STRBUF_HEIGHT	20

char StringBuf[ STRBUF_HEIGHT ][ STRBUF_WIDTH * 2 + 1 ] ;	// 仮想テキストバッファ
int CursorX , CursorY ;						// 仮想画面上での文字表示カーソルの位置
int SP , CP ;							// 参照する文字列番号と文字列中の文字ポインタ
int EndFlag ;							// 終了フラグ
int KeyWaitFlag ;						// ボタン押し待ちフラグ
int Count ;							// フレームカウンタ

char String[][ 256 ] =
{
	"　ゲームプログラムを習得するための一番の近道はとにかく沢山プログラムを組む",
	"ことである。B" ,
	"@　プログラムの参考書にはゲームのプログラムの方法なんて何も書かれていない、B",
	"変数、B配列、B関数、Bループ、B条件分岐…Bこれらすべての説明はゲームで何に使うか",
	"なんてどこにも書いていない、Bせいぜい住所録を題材にした例がある程度である。B" ,
	"C　プログラムは習うより慣れろなのでプログラムを組むに当たって少しでも知識が",
	"つけば後はそこからは掘り下げ、広げていけば良いわけで、Bプログラムの参考書を",
	"読んでいて少しでも何か出来るような気がしたらそこでとにかくプログラム",
	"を打ってみることが大事である。E",
} ;

void Kaigyou( void ) ;		// テキストバッファの改行処理関数


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						 LPSTR lpCmdLine, int nCmdShow )
{
	char OneMojiBuf[ 3 ] ;	// １文字分一時記憶配列
	int i , j ;

	SetGraphMode( 640 , 480 , 16 ) ;
	if( DxLib_Init() == -1 )	// ＤＸライブラリ初期化処理
	{
		 return -1;				// エラーが起きたら直ちに終了
	}

	// 描画位置の初期位置セット
	CursorX = 0 ;
	CursorY = 0 ;
	
	// 参照文字位置をセット
	SP = 0 ;	// １行目の
	CP = 0 ;	// ０文字

	// フォントのサイズセット
	SetFontSize( MOJI_SIZE ) ;

	// フォントの太さを変更
	SetFontThickness( 1 ) ;

	// フォントのタイプをアンチエイリアスフォントに変更
	ChangeFontType( DX_FONTTYPE_ANTIALIASING_EDGE_8X8 ) ;

	// 描画先を裏画面にセット
	SetDrawScreen( DX_SCREEN_BACK ) ;

	// フレームカウンタ初期化
	Count = 0 ;

	// ループ
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		// サウンドノベル風文字列描画処理を行う
		// ただし終了フラグが１だった場合は処理をしない
		if( EndFlag == 0 )
		{
			char  Moji ;

			// ボタン押し待ちフラグがたっていた場合はボタンが押されるまでここで終了
			if( KeyWaitFlag == 1 )
			{
				if( ProcessMessage() == 0 && CheckHitKeyAll() != 0 ) 
				{
					// ボタンが押されていたら解除
					KeyWaitFlag = 0 ;
				}
			}
			else
			{
				// 文字の描画
				Moji = String[ SP ][ CP ] ;
				switch( Moji )
				{
				case '@' :	// 改行文字

					// 改行処理および参照文字位置を一つ進める
					Kaigyou() ;
					CP ++ ;

					break ;

				case 'B' :	// ボタン押し待ち文字

					// ボタンが離されるまで待つ
					while( ProcessMessage() == 0 && CheckHitKeyAll() != 0 ){}

					// ボタン押し待ちフラグをたてる
					KeyWaitFlag = 1 ;
					CP ++ ;

					break ;

				case 'E' :	// 終了文字

					// 終了フラグを立てるおよび参照文字位置を一つ進める
					EndFlag = 1 ;
					CP ++ ;

					break ;

				case 'C' :	// クリア文字

					// 仮想テキストバッファを初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
					for( i = 0 ; i < STRBUF_HEIGHT ; i ++ )
					{
						for( j = 0 ; j < STRBUF_WIDTH * 2 ; j ++ )
						{
							StringBuf[ i ][ j ] = 0 ;
						}
					}

					CursorY = 0 ;
					CursorX = 0 ;
					CP ++ ;

					break ;

				default :	// その他の文字

					// １文字分抜き出す
					OneMojiBuf[ 0 ] = String[ SP ][ CP ] ;
					OneMojiBuf[ 1 ] = String[ SP ][ CP + 1 ] ;
					OneMojiBuf[ 2 ] = '\0' ;

					// １文字テキストバッファに代入
					StringBuf[ CursorY ][ CursorX * 2 ] = OneMojiBuf[ 0 ] ;
					StringBuf[ CursorY ][ CursorX * 2 + 1 ] = OneMojiBuf[ 1 ] ;

					// 参照文字位置を２バイト勧める
					CP += 2 ;

					// カーソルを一文字文進める
					CursorX ++ ;

					// テキストバッファ横幅からはみ出たら改行する
					if( CursorX >= STRBUF_WIDTH ) Kaigyou() ;

					break ;
				}

				// 参照文字列の終端まで行っていたら参照文字列を進める
				if( String[ SP ][ CP ] == '\0' )
				{
					SP ++ ;
					CP = 0 ;
				}
			}
		}

		// 画面のクリア
		ClsDrawScreen() ;

		// 背景エフェクトの描画
		{
			int Color ;

			Color = ( int )( sin( Count / 100.0 ) * 80.0 + 125.0 ) ;
			DrawBox( 0 , 0 , 640 , 480 , GetColor( Color , 0 , 0 ) , TRUE ) ;
			Count ++ ;
		}

		// テキストバッファの描画
		for( i = 0 ; i < STRBUF_HEIGHT ; i ++ )
		{
			DrawString( 8 , i * MOJI_SIZE , StringBuf[ i ] , GetColor( 255 , 255 , 255 ) ) ;
		}

		// 裏画面の内容を表画面に反映させる
		ScreenFlip() ;
	}

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;					// ソフトの終了
}



// 改行関数
void Kaigyou( void )
{
	// 描画行位置を一つ下げる
	CursorY ++ ;

	// 描画列を最初に戻す
	CursorX = 0 ;

	// もしテキストバッファ縦幅からはみ出るならテキストバッファを縦スクロールさせる
	if( CursorY >= STRBUF_HEIGHT )
	{
		int i , j ;

		for( i = 1 ; i < STRBUF_HEIGHT ; i ++ )
		{
			for( j = 0 ; j < STRBUF_WIDTH * 2 ; j ++ )
			{
				StringBuf[ i - 1 ][ j ] = StringBuf[ i ][ j ] ;
			}
		}

		// 描画行位置を一つあげる
		CursorY -- ;
	}
}
