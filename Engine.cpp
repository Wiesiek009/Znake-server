#include "Engine.hpp"

Serwer::Serwer()
{
	m_config.load();

	listener.listen(port);
}

void Serwer::mainLoop()
{
	while(state == State::waitRoom)
	{
		Player* player = new Player;

		if (listener.accept(*player) == sf::Socket::Done)
		{
			selector.add(*player);
			m_players.push_back(player);

			packet.clear();

			short id;
			std::string name;
			m_players[m_players.size() - 1]->receive(packet);
			packet >> id >> name;
			player->init(id, name);

			std::cout << "Player number " << id + 1 << " has connected!!! His name is: " << name << "\n";
		}
		else
			delete player;

		if(m_players.size() == m_config.m_players)
		{
			std::cout << "theoretically preparing" << std::endl;
		}
	}

	while(state == State::inGame)
	{
		for (std::vector<Player*>::iterator i = m_players.begin(); i != m_players.end(); i++)
		{
			Player& client = **i;

			packet.clear();
			client.receive(packet);

			sf::Vector2f pos;
			float rot;
			bool canDraw;
			bool exist;

			packet >> pos.x >> pos.y >> rot >> canDraw >> exist;

			std::cout << "Otrzymalem: " << pos.x << " --- " << pos.y << " --- " << rot << " --- " << canDraw << "\n";

		}
	}
}