#pragma once
#include	"Color.h"
#include	"File.h"

#include	<fstream>

class Font {
public:
	Font() : m_text(""), m_pos(0, 0), m_color(COLOR_BLACK) {}
	Font(std::string text, Vector2 pos, int color);

	virtual ~Font() { if (m_font != -1 && m_font != NULL) DeleteFontToHandle(m_font); }

	static void Draw(Vector2 pos, const char* font, Color c);
	static void Draw(Vector2 pos, std::string font, Color c);

	template<typename T> static void FDraw(Vector2 pos, int color, const char* type, const T& data);
	template<typename T> static void FDraw(Vector2 pos, int color, std::string type, const T& data);
	template<typename T> static void FDraw(Vector2 pos, Color c, const char* type, const T& data);
	template<typename T> static void FDraw(Vector2 pos, Color c, std::string type, const T& data);

	template<typename T> static void ExDraw(Vector2 pos, Vector2 size, int color, const char* type, const T& data);
	template<typename T> static void ExDraw(Vector2 pos, Vector2 size, int color, std::string type, const T& data);
	template<typename T> static void ExDraw(Vector2 pos, Vector2 size, Color c, const char* type, const T& data);
	template<typename T> static void ExDraw(Vector2 pos, Vector2 size, Color c, std::string type, const T& data);

	void MFDraw(Vector2 pos, const char* font, Color c) { DrawStringToHandle(pos.x, pos.y, font, c.color, m_font); }
	void MFDraw(Vector2 pos, std::string font, Color c) { DrawStringToHandle(pos.x, pos.y, font.c_str(), c.color, m_font); }

	void SetFont(const char* font_name, int size, int thick);
	void SetFont(const char* font_name, int size, int thick, int type);

	void SetText_All(const char* text, Vector2 pos, int color) { SetPos(pos); SetColor(color); SetText(text); }
	void SetText_All(std::string text, Vector2 pos, int color) { SetPos(pos); SetColor(color); SetText(text); }
	void SetPos(Vector2 pos) { m_pos = pos; }
	void SetColor(int color) { m_color.color = color; }
	void SetText(const char* text);
	void SetText(std::string text);

	void NormalDrawText() { DrawFormatString(m_pos.x, m_pos.y, m_color.color, "%s", m_text.c_str()); }
	void OrderDrawText();

	void ResetShow() { if (m_show != "\0") m_show.clear(); if (!m_byte_flag.empty()) m_byte_flag.clear(); m_show_index = 0; }
private:
	int m_font = NULL;

	Vector2 m_pos;
	Color m_color;
	std::string m_text;
	int m_show_index = 0;
	std::string m_show;
	std::vector<bool> m_byte_flag;
};

class Sound {
public:
	Sound(){}
	Sound(const std::string& file_name) : m_sound(file_name){}
	Sound(Sound&& other) noexcept : m_sound(std::move(other.m_sound)){}

	virtual ~Sound(){}

	void operator=(const Sound& other);
	const Sound& operator=(const Sound&& other) noexcept;

	void SetSound(const std::string& file_name) { m_sound.LoadFile(file_name); }
	void SetVolume(double raito);

	void Play(int type) { if (!CheckSoundMem(m_sound.GetSoundFile())) PlaySoundMem(m_sound.GetSoundFile(), type); m_type = type; }
	void Stop() { if (CheckSoundMem(m_sound.GetSoundFile())) StopSoundMem(m_sound.GetSoundFile()); }

	void Stop_And_Play();
private:
	int m_type = NULL;

	SoundFile m_sound;
};

class Image {
public:
	Image(){}
	Image(const std::string& file_name) : m_image(file_name) { GetGraphSize(m_image.GetImageFile(), (int*)&m_image_graphSize.x, (int*)&m_image_graphSize.y); }
	Image(Image&& other) noexcept : m_image(std::move(other.m_image)){ GetGraphSize(m_image.GetImageFile(), (int*)&m_image_graphSize.x, (int*)&m_image_graphSize.y);}

	virtual ~Image(){}

	void operator=(const Image& other);
	const Image& operator=(const Image&& other) noexcept;

	void Draw(const Vector2& pos);

	void ExDraw_Limit(const Vector2& pos, const Vector2 limit);
	void ExDraw_Scale(const Vector2& pos, const Vector2 scale);

	void SetImage(const std::string& file_name);

	bool CheckImageData() const { return m_image.CheckSetImageFile(); }
	ImageFile& Get_ImageData() { return m_image; }
	const ImageFile& Get_ImageData() const { return m_image; }

	Vector2 Get_ImageGraphSize() const { return m_image_graphSize; }
	Vector2 Get_H_ImageGraphSize() const { return Get_ImageGraphSize() / 2; }

private:
	Vector2 m_image_graphSize;

	ImageFile m_image;
};

class Movie {
public:
	Movie(){}
	Movie(const std::string& file_name) : m_movie(file_name){}
	Movie(Movie&& other) noexcept : m_movie(std::move(other.m_movie)){}

	virtual ~Movie(){}

	void operator=(const Movie& other);
	const Movie& operator=(const Movie&& other) noexcept;

	void SetVolume(double raito); //0%Å`100%ê›íË

	void Play(const Vector2& pos);

	void ExPlay_Scale(const Vector2& pos, const Vector2& scale); //î{ó¶Ç†ÇËÅEägèkÇ»Çµ(Vector2)

	void ExPlay_Limit(const Vector2& pos, const Vector2& limit_pos); //î{ó¶Ç»ÇµÅEägèkÇ†ÇË(Vector2)

	void ExPlay(const Vector2& pos, const Vector2& limit_pos, const Vector2& scale); //î{ó¶Ç†ÇËÅEägèkÇ†ÇË(Vector2)

	void SetMovie(const std::string& file_name) { m_movie.LoadFile(file_name); GetGraphSize(m_movie.GetMovieFile(), (int*)&m_movie_graphSize.x, (int*)&m_movie_graphSize.y); }

	void Stop() { if(GetMovieStateToGraph(m_movie.GetMovieFile())) PauseMovieToGraph(m_movie.GetMovieFile()); }

	void Stop_And_Play();

	bool CheckMovieData() const { return m_movie.CheckSetMovieFile(); }
	MovieFile& Get_MovieData() { return m_movie; }
	const MovieFile& Get_MovieData() const { return m_movie; }

	Vector2 Get_MovieGraphSize() const { return m_movie_graphSize; }
	Vector2 Get_H_MovieGraphSize() const { return Get_MovieGraphSize() / 2; }

private:
	Vector2 m_movie_graphSize;

	MovieFile m_movie;
};