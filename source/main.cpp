#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

constexpr int window_width = 832, window_height = 832;
constexpr int one_square_size = window_width / 8;
constexpr int one_piece_size = 42;
constexpr float scale = one_square_size * 0.9f / one_piece_size;
constexpr float x_offset = one_square_size * 0.1f / 2.f;
constexpr float y_offset = one_square_size * 0.1f / 2.f;
sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chess Puzzles", sf::Style::Titlebar | sf::Style::Close);

int board[64];
std::vector <int> answers;

void load_level(int number_of_level) {
	std::string line;
	std::ifstream level("assets/levels/" + std::to_string(number_of_level) + ".txt");


	int i = 0;
	while (getline(level, line) && i < 64)
	{
		board[i] = std::stoi(line);
		++i;
	}

	level.close();
}

void load_level_answers(int number_of_level) {
	std::string line;
	std::ifstream level("assets/answers/" + std::to_string(number_of_level) + ".txt");


	while (getline(level, line))
	{
		answers.push_back(std::stoi(line));
	}

	level.close();
}

void render_sprite_at_pos(sf::Sprite& sprite, int x, int y) {
	sprite.setPosition(sf::Vector2f(x, y));
	window.draw(sprite);
}

int main() {
	int square_1 = -1, square_2 = -1, i = 0, x, y, saved_piece, counter_of_moves = 0,counter_of_level = 1;
	bool first_move = true, correct_move = true;
	sf::Time time_to_sleep;
	sf::Vector2i mouse_pos;
	sf::Event event;
	sf::Image icon;
	icon.loadFromFile("assets/images/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window.setFramerateLimit(60);

	sf::Texture pawn_black;
	sf::Texture pawn_white;
	sf::Texture knight_black;
	sf::Texture knight_white;
	sf::Texture bishop_black;
	sf::Texture bishop_white;
	sf::Texture rook_black;
	sf::Texture rook_white;
	sf::Texture queen_black;
	sf::Texture queen_white;
	sf::Texture king_black;
	sf::Texture king_white;
	sf::Texture chess_board_texture;

	pawn_black.loadFromFile("assets/images/pawn_black.png");
	pawn_white.loadFromFile("assets/images/pawn_white.png");
	knight_black.loadFromFile("assets/images/knight_black.png");
	knight_white.loadFromFile("assets/images/knight_white.png");
	bishop_black.loadFromFile("assets/images/bishop_black.png");
	bishop_white.loadFromFile("assets/images/bishop_white.png");
	rook_black.loadFromFile("assets/images/rook_black.png");
	rook_white.loadFromFile("assets/images/rook_white.png");
	queen_black.loadFromFile("assets/images/queen_black.png");
	queen_white.loadFromFile("assets/images/queen_white.png");
	king_black.loadFromFile("assets/images/king_black.png");
	king_white.loadFromFile("assets/images/king_white.png");
	chess_board_texture.loadFromFile("assets/images/chess_board.jpg");

	sf::Sprite pawn_bl(pawn_black);
	sf::Sprite pawn_wh(pawn_white);
	sf::Sprite knight_bl(knight_black);
	sf::Sprite knight_wh(knight_white);
	sf::Sprite rook_bl(rook_black);
	sf::Sprite rook_wh(rook_white);
	sf::Sprite bishop_bl(bishop_black);
	sf::Sprite bishop_wh(bishop_white);
	sf::Sprite queen_bl(queen_black);
	sf::Sprite queen_wh(queen_white);
	sf::Sprite king_bl(king_black);
	sf::Sprite king_wh(king_white);
	sf::Sprite chess_board(chess_board_texture);

	pawn_bl.setScale(sf::Vector2f(scale, scale));
	pawn_wh.setScale(sf::Vector2f(scale, scale));
	knight_bl.setScale(sf::Vector2f(scale, scale));
	knight_wh.setScale(sf::Vector2f(scale, scale));
	rook_bl.setScale(sf::Vector2f(scale, scale));
	rook_wh.setScale(sf::Vector2f(scale, scale));
	bishop_bl.setScale(sf::Vector2f(scale, scale));
	bishop_wh.setScale(sf::Vector2f(scale, scale));
	queen_bl.setScale(sf::Vector2f(scale, scale));
	queen_wh.setScale(sf::Vector2f(scale, scale));
	king_bl.setScale(sf::Vector2f(scale, scale));
	king_wh.setScale(sf::Vector2f(scale, scale));
	chess_board.setScale(sf::Vector2f((float)window_width / chess_board_texture.getSize().x, (float)window_height / chess_board_texture.getSize().y));

	load_level(counter_of_level);  
	load_level_answers(counter_of_level);

	while (window.isOpen()) { 
		
		if (square_1 != -1 && square_2 != -1) {
			if (!correct_move) {
				board[square_1] = board[square_2];
				board[square_2] = saved_piece;
				time_to_sleep = sf::milliseconds(400);
				sf::sleep(time_to_sleep);
				goto reset_tiles;
			}
			time_to_sleep = sf::milliseconds(400);
			sf::sleep(time_to_sleep);
			if ((counter_of_moves + 1) * 4 > answers.size() - 1) {
				counter_of_level++;
				answers.clear();
				counter_of_moves = 0;
				load_level(counter_of_level);
				load_level_answers(counter_of_level);
				goto reset_tiles;
			}
			board[answers[4 * counter_of_moves + 3]] = board[answers[4 * counter_of_moves + 2]];
			board[answers[4 * counter_of_moves + 2]] = 0;
			counter_of_moves++;
			reset_tiles:
				square_1 = -1;
				square_2 = -1;

		}

		while (window.pollEvent(event)) {
			
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && first_move) {
				mouse_pos = sf::Mouse::getPosition(window);
				square_1 = (mouse_pos.x / one_square_size) + (mouse_pos.y / one_square_size) * 8;
				first_move = false;
			}

			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !first_move) {
				mouse_pos = sf::Mouse::getPosition(window);
				square_2 = (mouse_pos.x / one_square_size) + (mouse_pos.y / one_square_size) * 8;
				first_move = true;
			}
		}
		
		if (square_1 != -1 && square_2 != -1) {
			saved_piece = board[square_2];
			board[square_2] = board[square_1];
			board[square_1] = 0;
			correct_move = square_1 == answers[counter_of_moves * 4] && square_2 == answers[counter_of_moves * 4 + 1];
		}

		window.clear();

		window.draw(chess_board);

		for (int i = 0; i < 64; ++i) {
			if (board[i] != 0) {
				x = i % 8 * one_square_size + x_offset;
				y = i / 8 * one_square_size + y_offset;
				if (board[i] > 0) {
					if (board[i] == 1) {
						render_sprite_at_pos(pawn_wh, x, y);
					}
					else if (board[i] == 2) {
						render_sprite_at_pos(knight_wh, x, y);
					}
					else if (board[i] == 3) {
						render_sprite_at_pos(bishop_wh, x, y);
					}
					else if (board[i] == 4) {
						render_sprite_at_pos(rook_wh, x, y);
					}
					else if (board[i] == 5) {
						render_sprite_at_pos(queen_wh, x, y);
					}
					else if (board[i] == 6) {
						render_sprite_at_pos(king_wh, x, y);
					}
				}
				
				else if (board[i] < 0) {
					if (board[i] == -1) {
						render_sprite_at_pos(pawn_bl, x, y);
					}
					else if (board[i] == -2) {
						render_sprite_at_pos(knight_bl, x, y);
					}
					else if (board[i] == -3) {
						render_sprite_at_pos(bishop_bl, x, y);
					}
					else if (board[i] == -4) {
						render_sprite_at_pos(rook_bl, x, y);
					}
					else if (board[i] == -5) {
						render_sprite_at_pos(queen_bl, x, y);
					}
					else if (board[i] == -6) {
						render_sprite_at_pos(king_bl, x, y);
					}
				}
			}
		}
		
		window.display();
	}
}