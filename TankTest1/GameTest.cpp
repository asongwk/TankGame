#include "GameTest.h"

GameTest::GameTest(HINSTANCE hInstance, wstring wndName /*= L""*/, int wndWidth /*= 1024*/,
	int wndHeight /*= 768*/)
	:T_Engine(hInstance, wndName, wndWidth, wndHeight)
{
	srand(unsigned int(time(0)));
	m_menu = NULL;
	LoadGameMenu();
	m_gameState = GAME_START;
	
	m_map = NULL;
	m_level = 1;

	m_playerTank1 = NULL;
	m_playerTank2 = NULL;

	m_check = 0;

	m_soundFailSound = NULL;
	m_soundWinSound = NULL;
	m_soundMenuMusic = NULL;
	m_soundRunMusic = NULL;
	
	m_helpImg = NULL;
}

GameTest::~GameTest()
{
	DestoryGameMenu();
	DestoryGameMap();
	DestoryTank();
	DestoryGameSound();

	if (m_helpImg != NULL){
		delete m_helpImg;
		m_helpImg = NULL;
	}

}

void GameTest::GameCreate()
{
	LoadGameSound(m_hwnd);
	PlayGameSound(SOUND_MENU);
}

void GameTest::GamePaint(HDC hdc)
{
	RectF rec;
	switch (m_gameState)
	{
	case GAME_START:
		m_menu->DrawMenu(hdc);
		
		break;
	case GAME_RUN_SINGLE:
		GamePaintSinglePeople(hdc);
		break;
	case GAME_RUN_DOUBLE:
		GamePaintDoublePeople(hdc);
		break;
	case GAME_HELP:
		/*
		rec.X = 100;
		rec.Y = 100;
		rec.Width = 200;
		rec.Height = 50;
		T_Graph::PaintText(hdc, rec, L"GAME HELP", (REAL)20, L"宋体", Color::Red);
		*/
		GamePaintHelp(hdc);
		break;
	case GAME_OVER:
		return;
		break;
	}

}

void GameTest::GamePaintSinglePeople(HDC hdc)
{
	m_map->Draw(hdc);
	if (m_playerTank1 != NULL){
		m_playerTank1->Draw(hdc);
	}
	for (auto it : mv_npcTank){
		it->Draw(hdc);
	}
	for (auto it : mv_playerbullet){
		it->Draw(hdc);
	}
	for (auto it : mv_npcbullet){
		it->Draw(hdc);
	}
	vector<T_Explode*>::iterator it;
	for (it = mv_explode.begin(); it != mv_explode.end();){
		(*it)->Draw(hdc);
		//if ((*it)->GetFrames() == G_EXPLODE_FRAMES / 2){
		//	m_soundExplosionSound->Play(false);
		//}
		(*it)->IntoNextFrames();
		if ((*it)->GetFrames() == G_EXPLODE_FRAMES){
			it = mv_explode.erase(it);
		}
		else
		{
			it++;
		}
	}
	RectF rec;
	rec.X = 50;
	rec.Y = 10;
	rec.Width = 200;
	rec.Height = 30;
	wchar_t strlife[256];
	int life = 0;
	if (NULL != m_playerTank1){
		life = m_playerTank1->GetLife();
	}
	wsprintf(strlife, L"坦克生命值 ： %d", life);
	T_Graph::PaintText(hdc, rec, strlife, (REAL)12, L"宋体", Color::Red);
}

void GameTest::GamePaintDoublePeople(HDC hdc)
{
	m_map->Draw(hdc);

	if (m_playerTank1 != NULL){
		m_playerTank1->Draw(hdc);
	}

	if (m_playerTank2 != NULL){
		m_playerTank2->Draw(hdc);
	}
	


	for (auto it : mv_npcTank){
		it->Draw(hdc);
	}
	for (auto it : mv_playerbullet){
		it->Draw(hdc);
	}
	for (auto it : mv_npcbullet){
		it->Draw(hdc);
	}

	vector<T_Explode*>::iterator it;
	for (it = mv_explode.begin(); it != mv_explode.end();){
		(*it)->Draw(hdc);
		//if ((*it)->GetFrames() == G_EXPLODE_FRAMES / 2){
		//	m_soundExplosionSound->Play(false);
		//}
		(*it)->IntoNextFrames();
		if ((*it)->GetFrames() == G_EXPLODE_FRAMES){
			it = mv_explode.erase(it);
		}
		else
		{
			it++;
		}
	}

	RectF rec;
	rec.X = 50;
	rec.Y = 10;
	rec.Width = 200;
	rec.Height = 30;
	wchar_t strlife[256];
	wchar_t strgoldlife[256];

	int life = 0;
	int goldlife = 0;
	if (NULL != m_playerTank1){
		life = m_playerTank1->GetLife();
	}
	if (NULL != m_playerTank2){
		goldlife = m_playerTank2->GetLife();
	}
	wsprintf(strlife, L"红色坦克生命值 ： %d", life);
	wsprintf(strgoldlife, L"金色坦克生命值 ： %d", goldlife);

	T_Graph::PaintText(hdc, rec, strlife, (REAL)12, L"宋体", Color::Red);
	rec.X = m_wndWidth - rec.X - rec.Width;
	T_Graph::PaintText(hdc, rec, strgoldlife, (REAL)12, L"宋体", Color::Red);

}

void GameTest::GamePaintHelp(HDC hdc)
{
	if (m_helpImg == NULL){
		m_helpImg = new T_Graph();
		m_helpImg->LoadImageFile(L"res\\helpImg.jpg");
		m_helpImg->PaintImage(hdc, 0, 0);
	}
}

void GameTest::GameLogic()
{
	switch (m_gameState)
	{
	case GAME_RUN_SINGLE:
		GameLogicSinglePeople();
		break;
	case GAME_RUN_DOUBLE:
		GameLogicDoublePeople();
		break;
	}
}

void GameTest::GameLogicSinglePeople()
{
	WanderOfTank();
	UpdatedNpcTankFrames();
	if (m_playerTank1 != NULL && m_playerTank1->GetActive()){
		switch (m_playerTank1->GetDir())
		{
		case DIR_LEFT:
			ToLeft(m_playerTank1);
			break;
		case DIR_RIGHT:
			ToRight(m_playerTank1);
			break;
		case DIR_UP:
			ToUp(m_playerTank1);
			break;
		case DIR_DOWN:
			ToDown(m_playerTank1);
			break;
		}
	}
	UpdateBulletPosition();
	BulletHitTank();
	AddBulletForNpcTank();
	BulletHitBarrier();
	if (NULL == m_playerTank1 &&  mv_explode.size() == 0){
		::MessageBox(NULL, L"游戏失败，进入菜单", L"", MB_OK);
		::SendMessage(m_hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		return;
	}
	if (0 == mv_npcTank.size() && mv_explode.size() == 0){
		if (1 == m_level){
			::MessageBox(NULL, L"游戏成功，进入下一关", L"", MB_OK);
			ResetData();
			m_level = 2;
			DestoryGameMap();
			LoadGameMap();
			LoadTank();
			PlayGameSound(SOUND_RUN);
		}
		else if (2 == m_level){
			::MessageBox(NULL, L"游戏成功，进入下一关", L"", MB_OK);
			ResetData();
			m_level = 3;
			DestoryGameMap();
			LoadGameMap();
			LoadTank();
			PlayGameSound(SOUND_RUN);
		}
		else{
			m_level = 1;
			::MessageBox(NULL, L"游戏成功，返回菜单", L"", MB_OK);
			::SendMessage(m_hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		}
		//::SendMessage(m_hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	}
}

void GameTest::GameLogicDoublePeople()
{
	WanderOfTank();
	UpdatedNpcTankFrames();
	if (m_playerTank1 != NULL && m_playerTank1->GetActive()){
		switch (m_playerTank1->GetDir())
		{
		case DIR_LEFT:
			ToLeft(m_playerTank1);
			break;
		case DIR_RIGHT:
			ToRight(m_playerTank1);
			break;
		case DIR_UP:
			ToUp(m_playerTank1);
			break;
		case DIR_DOWN:
			ToDown(m_playerTank1);
			break;
		}
	}
	if (m_playerTank2 != NULL && m_playerTank2->GetActive()){
		switch (m_playerTank2->GetDir())
		{
		case DIR_LEFT:
			ToLeft(m_playerTank2);
			break;
		case DIR_RIGHT:
			ToRight(m_playerTank2);
			break;
		case DIR_UP:
			ToUp(m_playerTank2);
			break;
		case DIR_DOWN:
			ToDown(m_playerTank2);
			break;
		}
	}
	UpdateBulletPosition();
	BulletHitTank();
	AddBulletForNpcTank();
	BulletHitBarrier();
	if (m_playerTank1 == NULL && m_playerTank2 == NULL&&  mv_explode.size() == 0){
		::MessageBox(NULL, L"游戏失败，进入菜单", L"", MB_OK);
		::SendMessage(m_hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		return;

	}
	if (0 == mv_npcTank.size() && mv_explode.size() == 0){
		if (1 == m_level){
			::MessageBox(NULL, L"游戏成功，进入下一关", L"", MB_OK);
			ResetData();
			m_level = 2;
			DestoryGameMap();
			LoadGameMap();
			LoadTank();
			PlayGameSound(SOUND_RUN);
		}
		else if (2 == m_level){
			::MessageBox(NULL, L"游戏成功，进入下一关", L"", MB_OK);
			ResetData();
			m_level = 3;
			DestoryGameMap();
			LoadGameMap();
			LoadTank();
			PlayGameSound(SOUND_RUN);
		}
		else{
			m_level = 1;
			::MessageBox(NULL, L"游戏成功，进入菜单", L"", MB_OK);
			::SendMessage(m_hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		}
		//::SendMessage(m_hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	}

}

void GameTest::GameKeyAction(WPARAM wParam, bool isUp)
{
	//::OutputDebugString(L"--key action\n");
	if (m_gameState == GAME_RUN_SINGLE && m_playerTank1!= NULL){
		if(!isUp){
			switch (wParam)
			{
			case VK_LEFT:
				m_playerTank1->SetActive(true);
				m_playerTank1->SetDir(DIR_LEFT);
				break;
			case VK_RIGHT:
				m_playerTank1->SetActive(true);
				m_playerTank1->SetDir(DIR_RIGHT);
				break;
			case VK_UP:
				m_playerTank1->SetActive(true);
				m_playerTank1->SetDir(DIR_UP);
				break;
			case VK_DOWN:
				m_playerTank1->SetActive(true);
				m_playerTank1->SetDir(DIR_DOWN);
				break;
			case 45: //数字键盘‘0’键
				AddBullet(m_playerTank1);
				/*
				m_check = mv_playerbullet.size();
				wchar_t str[30];
				wsprintf(str,L"player bullet is %d\n",m_check);
				::OutputDebugString(str);
				*/
				break;
			}
		}
		else{
			switch (wParam)
			{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
				m_playerTank1->SetActive(false);
				break;
			}
		
		}
	}


	if (m_gameState == GAME_RUN_DOUBLE ){
		if (m_playerTank1 != NULL){
			if (KEYDOWN(VK_LEFT) || KEYDOWN(VK_RIGHT) || KEYDOWN(VK_UP) || KEYDOWN(VK_DOWN)){
				m_playerTank1->SetActive(true);
			}

			if (KEYUP(VK_LEFT) && KEYUP(VK_RIGHT) && KEYUP(VK_UP) && KEYUP(VK_DOWN)){
				m_playerTank1->SetActive(false);
			}

			if (KEYDOWN(VK_UP)){
				m_playerTank1->SetDir(DIR_UP);
			}

			if (KEYDOWN(VK_LEFT)){
				m_playerTank1->SetDir(DIR_LEFT);
			}

			if (KEYDOWN(VK_DOWN)){
				m_playerTank1->SetDir(DIR_DOWN);
			}

			if (KEYDOWN(VK_RIGHT)){
				m_playerTank1->SetDir(DIR_RIGHT);
			}
			if (KEYDOWN(45)){ /*45表示小键盘数字0键*/
				AddBullet(m_playerTank1);
			}
		}
		/*-------------------------------------------------------*/
		if (m_playerTank2 != NULL){
			if (KEYDOWN('A') || KEYDOWN('S') || KEYDOWN('D') || KEYDOWN('W')){
				m_playerTank2->SetActive(true);
			}

			if (KEYUP('A') && KEYUP('S') && KEYUP('D') && KEYUP('W')){
				m_playerTank2->SetActive(false);
			}

			if (KEYDOWN('W')){
				m_playerTank2->SetDir(DIR_UP);
			}

			if (KEYDOWN('A')){
				m_playerTank2->SetDir(DIR_LEFT);
			}

			if (KEYDOWN('S')){
				m_playerTank2->SetDir(DIR_DOWN);
			}

			if (KEYDOWN('D')){
				m_playerTank2->SetDir(DIR_RIGHT);
			}
			if (KEYDOWN(32)){  /*32表示空格键*/
				AddBullet(m_playerTank2);
			}
		}
	}
}

void GameTest::GameMouseAction(int x, int y, int action)
{
	if (action == MOUSE_MOVE){
		if (m_gameState == GAME_START){
			m_menu->SetMouseCoordinate(x, y);
			PlayGameSound(SOUND_MENU);
		}
	}
	else if (action == MOUSE_LCLICK){
		if (m_gameState == GAME_START){

			MENU_INDEX index = m_menu->GetMenuIndexMouseClick(x, y);
			if (index == -1 ) return;
			switch (index)
			{
			case MENU_SINGLE_PEOPLE:
				m_gameState = GAME_RUN_SINGLE;
				
				PlayGameSound(SOUND_RUN);

				LoadGameMap();
				LoadTank();
				break;
			case MENU_DOUBLE_PEOPLE:
				m_gameState = GAME_RUN_DOUBLE;
				
				PlayGameSound(SOUND_RUN);
				LoadGameMap();
				LoadTank();
				break;
			case MENU_HELP:
				m_gameState = GAME_HELP;
				break;
			case MENU_EXIT:
				::SendMessage(m_hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				break;
			}
		}
	}
}

void GameTest::GameEnd()
{

}

void GameTest::ResetData()
{
	m_level = 1;
	DestoryGameMap();
	DestoryTank();
	DestoryAllBullet();
	
}


/*---------------menu--------------------------------------------------*/
void GameTest::LoadGameMenu()
{
	m_menu = new T_Menu(m_wndWidth, m_wndHeight);
}

void GameTest::DestoryGameMenu()
{
	if (NULL != m_menu){
		delete m_menu;
		m_menu = NULL;
	}
}


/*---------------map--------------------------------------------------------*/
void GameTest::LoadGameMap()
{
	if (m_map != NULL)
		return;
	m_map = new T_Map();
	if (1 == m_level){
		m_map->LoadTmxFile("res\\game\\map1.tmx");
	}
	else if(2 == m_level){
		m_map->LoadTmxFile("res\\game\\map2.tmx");
	}
	else if (3 == m_level){
		m_map->LoadTmxFile("res\\game\\map3.tmx");
	}
}

void GameTest::DestoryGameMap()
{
	if (NULL != m_map){
		delete m_map;
		m_map = NULL;
	}
}


/*-------------------------tank---------------------------------------*/
void GameTest::LoadTank()
{

	m_playerTank1 = new T_Tank();
	m_playerTank1->SetImage(L"res\\sprite\\playertank.png");
	m_playerTank1->SetLife(3);
	m_playerTank1->SetGapTime(G_GAP_TIME_OF_TANK);
	Rect rec;
	rec = m_map->GetRectOfEmptyOnMap(m_playerTank1->GetWidth(), m_playerTank1->GetHeight());
	while (IsCollideOfTankAndMap(NULL,rec) == false){
		rec = m_map->GetRectOfEmptyOnMap(m_playerTank1->GetWidth(), m_playerTank1->GetHeight());
	}
	m_playerTank1->SetCoordinate(rec.X, rec.Y);
	m_playerTank1->SetSpeed(G_PLAYER_TANK_SPEED);
	m_playerTank1->SetDir(GAME_DIR(rand() % 4));
	m_playerTank1->SetActive(false);
	

	if (GAME_RUN_DOUBLE == m_gameState){
		m_playerTank2 = new T_Tank();
		m_playerTank2->SetImage(L"res\\sprite\\playersuper.png");
		m_playerTank2->SetLife(3);
		m_playerTank2->SetGapTime(G_GAP_TIME_OF_TANK);
		rec = m_map->GetRectOfEmptyOnMap(m_playerTank2->GetWidth(), m_playerTank2->GetHeight());
		while (IsCollideOfTankAndMap(NULL,rec) == false){
			rec = m_map->GetRectOfEmptyOnMap(m_playerTank2->GetWidth(), m_playerTank2->GetHeight());
		}
		m_playerTank2->SetCoordinate(rec.X+200, rec.Y);
		m_playerTank2->SetSpeed(G_PLAYER_TANK_SPEED);
		m_playerTank2->SetDir(GAME_DIR(rand() % 4));
		m_playerTank2->SetActive(false);
		
	}
	else{
		m_playerTank2 = NULL;
	}

	switch (m_level)
	{
	case 1:
		m_countOfNpc = G_COUNT_NPC_TANK_AT1;
		break;
	case 2:
		m_countOfNpc = G_COUNT_NPC_TANK_AT2;
		break;
	case 3:
		m_countOfNpc = G_COUNT_NPC_TANK_AT3;
		break;
	}

	
	for (int i = 0; i < m_countOfNpc;i++){
		T_Tank *t = new T_Tank();
		t->SetImage(L"res\\sprite\\npctank.png");
		Rect rec;
		rec = m_map->GetRectOfEmptyOnMap(t->GetWidth(), t->GetHeight());
		while (true){
			if (!IsCollideOfTankAndMap(NULL,rec)){
				break;
			}
			else{
				rec = m_map->GetRectOfEmptyOnMap(t->GetWidth(), t->GetHeight());
			}
		}

		wchar_t str[30];
		//wsprintf(str,L"\t----");
		//::OutputDebugString(str);

		t->SetCoordinate(rec.X, rec.Y);
		t->SetSpeed(G_NPC_TANK_SPEED);
		t->SetDir(GAME_DIR(rand()%4));
		t->SetGapTime(G_GAP_TIME_OF_TANK *3);
		mv_npcTank.push_back(t);
		t = NULL;
	}
}

void GameTest::DestoryTank()
{
	if (m_playerTank1!=NULL){
		delete m_playerTank1;
		m_playerTank1 = NULL;
	}
	if (m_playerTank2 != NULL){
		delete m_playerTank2;
		m_playerTank2 = NULL;
	}
	for (int i = mv_npcTank.size()-1; i >= 0;i--){
		T_Tank *t = mv_npcTank[i];
		mv_npcTank[i] = NULL;
		delete t;		
	}
	mv_npcTank.clear();
	m_countOfNpc = 0;
	//m_countOfPlayer = 0;
}

void GameTest::WanderOfTank() 
{
	for (auto it:mv_npcTank){
		Rect rec = it->GetNextTankRect();
		if (IsCollideOfTankAndMap(it,rec)){
			it->SetDir(GAME_DIR(rand() % 4));
		}
		else{
			it->SetCoordinate(rec.X, rec.Y);
		}
	}
}

void GameTest::ToLeft(T_Tank * t) 
{
	Rect rec = t->GetNextTankRect();
	if (!IsCollideOfTankAndMap(t, rec)){
		t->SetCoordinate(rec.X,rec.Y);
	}
}

void GameTest::ToRight(T_Tank * t)
{
	Rect rec = t->GetNextTankRect();
	if (!IsCollideOfTankAndMap(t, rec)){
		t->SetCoordinate(rec.X, rec.Y);
	}
}

void GameTest::ToUp(T_Tank *t)
{
	Rect rec = t->GetNextTankRect();
	if (!IsCollideOfTankAndMap(t, rec)){
		t->SetCoordinate(rec.X, rec.Y);
	}
}

void GameTest::ToDown(T_Tank *t)
{
	Rect rec = t->GetNextTankRect();
	if (!IsCollideOfTankAndMap(t, rec)){
		t->SetCoordinate(rec.X, rec.Y);
	}
}

bool GameTest::IsCollideOfTankAndMap(T_Tank* t, Rect rec)
{
	if (m_map->IsCollide(rec))
		return true;
	int x = rec.X;
	int y = rec.Y;
	int tx, ty;
	int width = rec.Width;
	int height = rec.Height;
	if (m_playerTank1!= NULL && m_playerTank1->IsVisible() && t != m_playerTank1){
		tx = m_playerTank1->GetCoordinateX();
		ty = m_playerTank1->GetCoordinateY();
		if (IsOverlapBetweenTwoRect(x,y,tx,ty,width,height) ){
			return true;
		}
	}
	if (m_playerTank2!= NULL && m_playerTank2->IsVisible() && t!= m_playerTank2){
		tx = m_playerTank2->GetCoordinateX();
		ty = m_playerTank2->GetCoordinateY();
		if (IsOverlapBetweenTwoRect(x, y, tx, ty, width, height)){
			return true;
		}
	}

	for (auto it : mv_npcTank){
		tx = it->GetCoordinateX();
		ty = it->GetCoordinateY();
		if (it == t){
			continue;
		}
		if (IsOverlapBetweenTwoRect(x, y, tx, ty, width, height)){
			return true;
		}
	}
	return false;
}

bool GameTest::IsOverlapBetweenTwoRect(int x1, int y1, int x2, int y2,int width, int height,int w2/*=0*/,int h2/*=0*/)
{
	if (0 == w2){
		int w = abs(x1 - x2);
		int h = abs(y1 - y2);
		if (w < width && h < height)
		{
			return true;
		}
		else{
			return false;
		}
	}
	else{
		x1 = x1 + width / 2;
		y1 = y1 + height / 2;
		x2 = x2 + w2 / 2;
		y2 = y2 + h2 / 2;
		int w = abs(x1 - x2);
		int h = abs(y1 - y2);
		if (w < (width+w2)/2 && h < (height +h2)/2){
			return true;
		}
		else {
			return false;
		}
	}

}

void GameTest::UpdatedNpcTankFrames()
{
	for (auto it:mv_npcTank){
		it->IntoNextFrames();
	}
}


/*------------------bullet---------------------------------------------------*/
void GameTest::AddBullet(T_Tank * t)
{
	if (t == NULL)return;
	DWORD nowTime = GetTickCount();
	DWORD lastTime = t->GetLastTime();
	int x = -1, y = -1;
	if (lastTime == 0 || nowTime - lastTime > t->GetGapTime()){
		x = t->GetCoordinateX() + t->GetWidth() / 2;
		y = t->GetCoordinateY() + t->GetHeight() / 2;
		t->SetLastBulletTime(nowTime);
	}
	else{
		return;
	}
	T_Bullet * b = new T_Bullet();
	b->SetCoordinate(x,y);
	b->SetDir(t->GetDir());
	b->SetSpeed(t->GetSpeed()*G_BULLET_SPEED);

	if (t == m_playerTank1 || t==m_playerTank2){
		switch (b->GetDir())
		{
		case DIR_LEFT:
			b->LoadBulletImage(L"res\\sprite\\bulletleft.png");
			break;
		case DIR_RIGHT:
			b->LoadBulletImage(L"res\\sprite\\bulletright.png");
			break;
		case DIR_UP:
			b->LoadBulletImage(L"res\\sprite\\bulletup.png");
			break;
		case DIR_DOWN:
			b->LoadBulletImage(L"res\\sprite\\bulletdown.png");
			break;
		}
		mv_playerbullet.push_back(b);
	}
	else{
		switch (b->GetDir())
		{
		case DIR_LEFT:
			b->LoadBulletImage(L"res\\sprite\\npcbulletleft.bmp");
			break;
		case DIR_RIGHT:
			b->LoadBulletImage(L"res\\sprite\\npcbulletright.bmp");
			break;
		case DIR_UP:
			b->LoadBulletImage(L"res\\sprite\\npcbulletup.bmp");
			break;
		case DIR_DOWN:
			b->LoadBulletImage(L"res\\sprite\\npcbulletdown.bmp");
			break;
		}
		mv_npcbullet.push_back(b);
	}
}

void GameTest::UpdateBulletPosition()
{
	vector<T_Bullet*>::iterator it;
	for (it = mv_playerbullet.begin(); it != mv_playerbullet.end();){
		Rect rec = (*it)->GetNextRect();
		int x = rec.X;
		int y = rec.Y;
		if (x < -(*it)->GetWidth() || y < -(*it)->GetHeight()
			|| x>m_map->GetWidth() || y > m_map->GetHeight()){
			T_Bullet * b = *it;
			it = mv_playerbullet.erase(it);
			delete b;
		}
		else{
			(*it)->SetCoordinate(x, y);
			it++;
		}
	}

	for (it = mv_npcbullet.begin(); it != mv_npcbullet.end();){
		Rect rec = (*it)->GetNextRect();
		int x = rec.X;
		int y = rec.Y;
		if (x < -(*it)->GetWidth() || y < -(*it)->GetHeight()
			|| x>m_map->GetWidth() || y > m_map->GetHeight()){
			T_Bullet * b = *it;
			it = mv_npcbullet.erase(it);
			delete b;
		}
		else{
			(*it)->SetCoordinate(x, y);
			it++;
		}
	}
}

void GameTest::BulletHitTank()
{
	vector<T_Bullet*>::iterator itb;
	vector<T_Tank*>::iterator itt;
	
	for (itb = mv_playerbullet.begin(); itb != mv_playerbullet.end();){
		bool bcollide = false;
		Rect rec1;
		rec1 = (*itb)->GetCurrentRect();
		for (itt = mv_npcTank.begin(); itt != mv_npcTank.end();){
			Rect rec2;
			rec2 = (*itt)->GetCurrentTankRect();
			if (IsOverlapBetweenTwoRect(rec1.X,rec1.Y,rec2.X,rec2.Y,rec1.Width,rec1.Height,rec2.Width,rec2.Height)){
				bcollide = true;
				(*itt)->DecreaseLief();
				if ((*itt)->IsDead()){ 
					T_Explode * e = new T_Explode();
					e->LoadExplodeImg(L"res\\sprite\\explosion.png");
					int xx = (*itt)->GetCoordinateX() + (*itt)->GetWidth() / 2;
					int yy = (*itt)->GetCoordinateY() + (*itt)->GetHeight() / 2;
					e->SetCoordinate(xx - e->GetWidth() / 2, yy - e->GetHeight() / 2);
					mv_explode.push_back(e);
					m_soundExplosionSound->Play(false);

					T_Tank * tt = *itt;
					itt = mv_npcTank.erase(itt);
					delete tt;
				}
				else{
					itt++;
				}
				break;
			}
			else{
				itt++;
			}
		}
		if (bcollide == false){
			itb++;
		}
		else{
			T_Bullet * bb = *itb;
			itb = mv_playerbullet.erase(itb);
			delete bb;
			bcollide = true;
		}
	}

	
	for (itb = mv_npcbullet.begin(); itb != mv_npcbullet.end();){
		bool bcollide = false;
		Rect rec1;
		rec1 = (*itb)->GetCurrentRect();
		if (m_playerTank1 != NULL){
			Rect rec2;
			rec2 = m_playerTank1->GetCurrentTankRect();
			if (IsOverlapBetweenTwoRect(rec1.X, rec1.Y, rec2.X, rec2.Y, rec1.Width, rec1.Height, rec2.Width, rec2.Height)){
				bcollide = true;
				m_playerTank1->DecreaseLief();
				if (m_playerTank1->IsDead())
				{
					T_Explode * e = new T_Explode();
					e->LoadExplodeImg(L"res\\sprite\\explosion.png");
					int xx = m_playerTank1->GetCoordinateX() + m_playerTank1->GetWidth() / 2;
					int yy = m_playerTank1->GetCoordinateY() + m_playerTank1->GetHeight() / 2;
					e->SetCoordinate(xx - e->GetWidth() / 2, yy - e->GetHeight() / 2);
					mv_explode.push_back(e);
					//m_soundExplosionSound->Play(false);

					delete m_playerTank1;
					m_playerTank1 = NULL;
				}
			}

		}

		if (bcollide == false && m_playerTank2!= NULL){
			Rect rec2;
			rec2 = m_playerTank2->GetCurrentTankRect();
			if (IsOverlapBetweenTwoRect(rec1.X, rec1.Y, rec2.X, rec2.Y, rec1.Width, rec1.Height, rec2.Width, rec2.Height)){
				bcollide = true;
				m_playerTank2->DecreaseLief();
				if (m_playerTank2->IsDead())
				{
					T_Explode * e = new T_Explode();
					e->LoadExplodeImg(L"res\\sprite\\explosion.png");
					int xx = m_playerTank2->GetCoordinateX() + m_playerTank2->GetWidth() / 2;
					int yy = m_playerTank2->GetCoordinateY() + m_playerTank2->GetHeight() / 2;
					e->SetCoordinate(xx - e->GetWidth() / 2, yy - e->GetHeight() / 2);
					mv_explode.push_back(e);
					m_soundExplosionSound->Play(false);

					delete m_playerTank2;
					m_playerTank2 = NULL;
				}
			}
		}

		if (bcollide == false){
			itb++;
		}
		else{
			T_Bullet * bb = *itb;
			itb = mv_npcbullet.erase(itb);
			delete bb;
			bcollide = true;
		}
	}

}

void GameTest::AddBulletForNpcTank()
{
	int t = 0;
	vector<T_Tank*> ::iterator it;
	for (it = mv_npcTank.begin(); it != mv_npcTank.end();it++){
		t = rand() % G_TIME_OF_NPC_BULLET;
		if (t == 1){
			AddBullet(*it);
		}
		t = 0;
	}
}

void GameTest::BulletHitBarrier()
{
	vector<T_Bullet*> ::iterator it;
	for (it = mv_playerbullet.begin(); it != mv_playerbullet.end();){
		Rect rec = (*it)->GetCurrentRect();
		if (m_map->IsCollide(rec)){
			POINT p = m_map->UpdateMapBarrier(rec.X, rec.Y, rec.Width, rec.Height);
			if (p.x >= 0 && p.y >= 0){
				T_Explode *e = new T_Explode();
				e->LoadExplodeImg(L"res\\sprite\\explosion.png");
				e->SetCoordinate(p.x, p.y);
				mv_explode.push_back(e);
				m_soundExplosionSound->Play(false);

				T_Bullet *b = *it;
				it = mv_playerbullet.erase(it);
				delete b;
			}
			else{
				it++;
			}
		}
		else{
			it++;
		}
	}
	for (it = mv_npcbullet.begin(); it != mv_npcbullet.end();){
		Rect rec = (*it)->GetCurrentRect();
		if (m_map->IsCollide(rec)){
			POINT p = m_map->UpdateMapBarrier(rec.X, rec.Y, rec.Width, rec.Height);
			if (p.x >=0 && p.y >=0){
				T_Explode *e = new T_Explode();
				e->LoadExplodeImg(L"res\\sprite\\explosion.png");
				e->SetCoordinate(p.x, p.y);
				mv_explode.push_back(e);
				//m_soundExplosionSound->Play(false);

				T_Bullet *b = *it;
				it = mv_npcbullet.erase(it);
				delete b;
			}
			else
			{
				it++;
			}
		}
		else{
			it++;
		}
	}
}

void GameTest::DestoryAllBullet()
{
	vector<T_Bullet*>::iterator it;
	for (it = mv_playerbullet.begin(); it != mv_playerbullet.end(); ){
		T_Bullet * b = *it;
		it = mv_playerbullet.erase(it);
		delete b;
	}

	for (it = mv_npcbullet.begin(); it != mv_npcbullet.end(); ){
		T_Bullet * b = *it;
		it = mv_npcbullet.erase(it);
		delete b;
	}
}


/*--------------------mode change---------------------------------------*/
void GameTest::ModeUpgrade(HDC hdc)
{

}

void GameTest::ModeWin(HDC hdc)
{

}

void GameTest::ModeFail(HDC hdc)
{

}

void GameTest::LoadModeChange()
{
	m_upgradeImg = new T_Graph();
	m_upgradeImg->CreateBlankBitmap(m_wndWidth, m_wndHeight, Color::Black);

	m_winImg = new T_Graph();
	m_winImg->CreateBlankBitmap(m_wndWidth, m_wndHeight, Color::Black);

	m_failImg = new T_Graph();
	m_failImg->CreateBlankBitmap(m_wndWidth, m_wndHeight, Color::Black);


}

void GameTest::DestroyModeChange()
{
	if (m_upgradeImg != NULL){
		delete m_upgradeImg;
	}
	if (m_winImg != NULL){
		delete m_winImg;
	}
	if (m_failImg != NULL){
		delete m_failImg;
	}
}

/*------------sound-------------------------------------------------------------*/
void GameTest::LoadGameSound(HWND hwnd)
{

	m_soundFailSound = new AudioDXBuffer;
	m_soundWinSound = new AudioDXBuffer;
	m_soundMenuMusic = new AudioDXBuffer;
	m_soundRunMusic = new AudioDXBuffer;
	m_soundTurnSound = new AudioDXBuffer;
	m_soundExplosionSound = new AudioDXBuffer;

	if (!m_dxSnd.CreateDS(hwnd))
		return;
	m_soundFailSound->LoadWave(m_dxSnd, L"res\\sound\\gameover.wav");
	m_soundWinSound->LoadWave(m_dxSnd, L"res\\sound\\victory.wav");
	m_soundMenuMusic->LoadWave(m_dxSnd, L"res\\sound\\menumusic.wav");
	m_soundRunMusic->LoadWave(m_dxSnd, L"res\\sound\\backmusic.wav");
	m_soundTurnSound->LoadWave(m_dxSnd, L"res\\sound\\mousedown.wav");
	m_soundExplosionSound->LoadWave(m_dxSnd, L"res\\sound\\blast.wav");
}

void GameTest::ReleaseDXBuffer(AudioDXBuffer* dxBuf)
{
	if (dxBuf != NULL)
	{
		dxBuf->Release();
		delete dxBuf;
		dxBuf = NULL;
	}
}

void GameTest::DestoryGameSound()
{
	ReleaseDXBuffer(m_soundFailSound);
	ReleaseDXBuffer(m_soundWinSound);
	ReleaseDXBuffer(m_soundMenuMusic);
	ReleaseDXBuffer(m_soundRunMusic);
	ReleaseDXBuffer(m_soundTurnSound);
	ReleaseDXBuffer(m_soundExplosionSound);

	m_dxSnd.ReleaseAll();
}

void GameTest::PlayGameSound(SOUND_INDEX index)
{

	if (m_soundFailSound == NULL)return;
	if (m_soundWinSound == NULL)return;
	if (m_soundMenuMusic == NULL)return;
	if (m_soundRunMusic == NULL)return;
		
	m_soundFailSound->Terminate();
	m_soundWinSound->Terminate(); 
	m_soundMenuMusic->Terminate();
	m_soundRunMusic->Terminate(); 

	switch (index)
	{
	case SOUND_MENU:
		m_soundMenuMusic->Play(true);
		break;
	case SOUND_RUN:
		m_soundRunMusic->Play(false);
		break;
	case SOUND_FAIL:
		m_soundFailSound->Play(false);
		break;
	case SOUND_WIN:
		m_soundWinSound->Play(false);
		break;
	}
}