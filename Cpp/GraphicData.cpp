#include	<cstdlib>
#include	<DxLib.h>

#include	"../h/ErrorMessage.h"
#include	"../h/GraphicData.h"

Font::Font(std::string text, Vector2 pos, int color) : m_text(text), m_pos(pos), m_color(color), m_show("\0") {
	for (int i = 0; m_text.size(); i++)
		m_byte_flag.push_back((m_text[i] >= 0x00 && m_text[i] <= 0x7E) || (m_text[i] >= 0xFF61 && m_text[i] <= 0xFF9F));
}

void Font::SetFont(const char* font_name, int size, int thick) {
	if (m_font != -1 && m_font != NULL) DeleteFontToHandle(m_font);

	m_font = CreateFontToHandle(font_name, size, thick);
}

void Font::SetFont(const char* font_name, int size, int thick, int type) {
	if (m_font != -1 && m_font != NULL) DeleteFontToHandle(m_font);

	m_font = CreateFontToHandle(font_name, size, thick, type);
}

void Font::Draw(Vector2 pos, const char* font, Color c) {
	DrawString(pos.x, pos.y, font, c.color);
}
void Font::Draw(Vector2 pos, std::string font, Color c) {
	DrawString(pos.x, pos.y, font.c_str(), c.color);
}

void Font::SetText(const char* text) {
	ResetShow();

	m_text = text;

	for (int i = 0; i < m_text.size(); i++)
		m_byte_flag.push_back((m_text[i] >= 0x00 && m_text[i] <= 0x7E) || (m_text[i] >= 0xFF61 && m_text[i] <= 0xFF9F));
}

void Font::SetText(std::string text) {
	ResetShow();

	m_text = text;

	for (int i = 0; m_text.size(); i++)
		m_byte_flag.push_back((m_text[i] >= 0x00 && m_text[i] <= 0x7E) || (m_text[i] >= 0xFF61 && m_text[i] <= 0xFF9F));
}

void Font::OrderDrawText() {
	if (m_text != "\0") {
		if (m_show_index < m_text.size() - 1) {
			if (!m_byte_flag[m_show_index]) m_show += m_text[m_show_index++];
			m_show += m_text[m_show_index++];
		}

		DrawFormatString(m_pos.x, m_pos.y, m_color.color, "%s", m_show.c_str());
		WaitTimer(128);
	}
}

void Sound::operator=(const Sound& other) { this->m_sound = other.m_sound; }

const Sound& Sound::operator=(const Sound&& other) noexcept {
	this->m_sound = other.m_sound;

	return *this;
}

void Sound::SetVolume(double raito){
	ChangeVolumeSoundMem(int(255 * raito / 100), m_sound.GetSoundFile()); //0～255をまでをraito÷100で求めた倍率にする。
}

void Sound::Stop_And_Play() {
	SetFontSize(50);

	if (CheckSoundMem(m_sound.GetSoundFile()) == 0) {
		PlaySoundMem(m_sound.GetSoundFile(), m_type);
		DrawString(0, 0, "再生", 0x00ff00);
	}
	else if (CheckSoundMem(m_sound.GetSoundFile()) == 1 && m_type != NULL) {
		StopSoundMem(m_sound.GetSoundFile());
		DrawString(0, 0, "停止", 0xffffff);
	}

	SetFontSize(20);
}
void Image::operator=(const Image& other){
	this->m_image = other.m_image;

	GetGraphSize(m_image.GetImageFile(), (int*)&m_image_graphSize.x, (int*)&m_image_graphSize.y);
}

const Image& Image::operator=(const Image&& other) noexcept {
	this->m_image = other.m_image;

	GetGraphSize(m_image.GetImageFile(), (int*)&m_image_graphSize.x, (int*)&m_image_graphSize.y);

	return *this;
}

void Image::Draw(const Vector2& pos) {
	if(!m_image.CheckSetImageFile()) throw ErrorMassage("画像が設定されてないため、描画することができませんでした。");

	Vector2 half = m_image_graphSize / 2;
	DrawGraph(int(pos.x) - half.x, int(pos.y) - half.y, m_image.GetImageFile(), TRUE);
}

void Image::ExDraw_Limit(const Vector2& pos, const Vector2 limit) {
	if (!m_image.CheckSetImageFile()) throw ErrorMassage("画像が設定されてないため、描画することができませんでした。");

	DrawExtendGraph(int(pos.x), int(pos.y), int(limit.x), int(limit.y), m_image.GetImageFile(), TRUE);
}

void Image::ExDraw_Scale(const Vector2& pos, const Vector2 scale) {
	if (!m_image.CheckSetImageFile()) throw ErrorMassage("画像が設定されてないため、描画することができませんでした。");

	Vector2 r = { m_image_graphSize.x * scale.x / 2, m_image_graphSize.y * scale.y / 2 }; //倍率に合わせてサイズを変える

	DrawExtendGraph(int(pos.x), int(pos.y), int(pos.x) + int(r.x), int(pos.y) + int(r.y), m_image.GetImageFile(), TRUE);
}

void Image::SetImage(const std::string& file_name) {
	m_image.Destroy();

	m_image.LoadFile(file_name);

	int x = m_image_graphSize.x, y = m_image_graphSize.y;
	GetGraphSize(m_image.GetImageFile(), &x, &y);

	m_image_graphSize = Vector2(x, y);
}

void Movie::operator=(const Movie& other) {
	this->m_movie.operator=(other.m_movie);

	GetGraphSize(m_movie.GetMovieFile(), (int*)&m_movie_graphSize.x, (int*)&m_movie_graphSize.y);
}

const Movie& Movie::operator=(const Movie&& other) noexcept {
	if (this != &other) this->m_movie.operator=(other.m_movie);

	GetGraphSize(m_movie.GetMovieFile(), (int*)&m_movie_graphSize.x, (int*)&m_movie_graphSize.y);

	return *this;
}

void Movie::SetVolume(double raito) {
	SetMovieVolumeToGraph(int(10000 * raito / 100), m_movie.GetMovieFile()); //0～10000までをraito÷100で求めた倍率にする。
}

void Movie::Play(const Vector2& pos) {
	if (!m_movie.CheckSetMovieFile()) throw ErrorMassage("Error::映像データが見つからないため、再生することができませんでした。");

	int nowFrame = TellMovieToGraphToFrame(m_movie.GetMovieFile());
	if (nowFrame >= m_movie.GetFrame()) { SeekMovieToGraph(m_movie.GetMovieFile(), 0); PlayMovieToGraph(m_movie.GetMovieFile()); } //再生位置を初期位置に戻して再生
	else if (nowFrame == 0) PlayMovieToGraph(m_movie.GetMovieFile()); //再生

	Vector2 p = { pos.x - m_movie_graphSize.x / 2, pos.y - m_movie_graphSize.y / 2 };

	DrawGraph(p.x, p.y, m_movie.GetMovieFile(), TRUE);
}

void Movie::ExPlay_Scale(const Vector2& pos, const Vector2& scale) {
	if (!m_movie.CheckSetMovieFile()) throw ErrorMassage("Error::映像データが見つからないため、再生することができませんでした。");

	int nowFrame = TellMovieToGraphToFrame(m_movie.GetMovieFile());
	if (nowFrame >= m_movie.GetFrame()) { SeekMovieToGraph(m_movie.GetMovieFile(), 0); PlayMovieToGraph(m_movie.GetMovieFile()); } //再生位置を初期位置に戻して再生
	else if (nowFrame == 0) PlayMovieToGraph(m_movie.GetMovieFile()); //再生

	Vector2 r = { m_movie_graphSize.x * scale.x / 2, m_movie_graphSize.y * scale.y / 2 };

	DrawExtendGraph(int(pos.x) - r.x, int(pos.y) - r.y, int(pos.x) + r.x, int(pos.y) + r.y, m_movie.GetMovieFile(), TRUE);
}
void Movie::ExPlay_Limit(const Vector2& pos, const Vector2& limit_pos) {
	if (!m_movie.CheckSetMovieFile()) throw ErrorMassage("Error::映像データが見つからないため、再生することができませんでした。");

	int nowFrame = TellMovieToGraphToFrame(m_movie.GetMovieFile());
	if (nowFrame >= m_movie.GetFrame()) { SeekMovieToGraph(m_movie.GetMovieFile(), 0); PlayMovieToGraph(m_movie.GetMovieFile()); } //再生位置を初期位置に戻して再生
	else if (nowFrame == 0) PlayMovieToGraph(m_movie.GetMovieFile()); //再生

	Vector2 halfPos = m_movie_graphSize / 2;

	DrawExtendGraph(int(pos.x) - halfPos.x, int(pos.y) - halfPos.y,
		int(limit_pos.x) + halfPos.x, int(limit_pos.y) + halfPos.y, m_movie.GetMovieFile(), TRUE);
}

void Movie::ExPlay(const Vector2& pos, const Vector2& limit_pos, const Vector2& scale) {
	if (!m_movie.CheckSetMovieFile()) throw ErrorMassage("Error::映像データが見つからないため、再生することができませんでした。");

	int nowFrame = TellMovieToGraphToFrame(m_movie.GetMovieFile());
	if (nowFrame >= m_movie.GetFrame()) { SeekMovieToGraph(m_movie.GetMovieFile(), 0); PlayMovieToGraph(m_movie.GetMovieFile()); } //再生位置を初期位置に戻して再生
	else if (nowFrame == 0) PlayMovieToGraph(m_movie.GetMovieFile()); //再生

	Vector2 r = { m_movie_graphSize.x * scale.x / 2, m_movie_graphSize.y * scale.y / 2 };

	DrawExtendGraph(int(pos.x) - r.x, int(pos.y) - r.y, int(limit_pos.x) + r.x, int(limit_pos.y) + r.y, m_movie.GetMovieFile(), TRUE);
}

void Movie::Stop_And_Play() {
	SetFontSize(50);

	if (GetMovieStateToGraph(m_movie.GetMovieFile()) == 0) {
		PlayMovieToGraph(m_movie.GetMovieFile());
		DrawString(0, 0, "再生", 0x00ff00);
	}

	else if (GetMovieStateToGraph(m_movie.GetMovieFile()) == 1) {
		PauseMovieToGraph(m_movie.GetMovieFile());
		DrawString(0, 0, "停止", 0xffffff);
	}

	SetFontSize(20);
}
