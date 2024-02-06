#pragma once

class PlayerData
{
public:
	PlayerData()
	{

	}

	PlayerData(Array<String, std::allocator<String>> _csv)
	{
		playerName = _csv[0];

		for (int i = 1; i < NumberOfPlayerScore + 1; i++)
		{
			playerScore << Parse<int32>(_csv[i]);
			hitBoxs << Rect{ 0,0,CellSize };
		}
	}

	const Size CellSize = { 170,42 };

	const int NumberOfPlayerScore = 6;
	int currentNumber;

	String playerName;
	Array<int32> playerScore;
	Array<Rect> hitBoxs;

	static Vec2 screenHomePos;

	void Update()
	{
		for (int i = 0;i < NumberOfPlayerScore;i++)
		{
			//マウスクリックでスコア増減
			if (hitBoxs[i].mouseOver())
			{
				if (MouseL.down())playerScore[i] ++;
				if (MouseR.down())if (playerScore[i] > 0)playerScore[i] --;
			}
		}
	}

	Array<String> GetData()
	{
		Array<String> data;

		data << playerName;

		for (auto d:playerScore)
		{
			data << Format(d);
		}

		return data;
	}

	void Draw(Font& font, int yPos)
	{
		font(playerName).draw(0, yPos, Palette::Black);
		for (int i = 0; i < NumberOfPlayerScore; i++)
		{
			font(playerScore[i]).draw(CellSize.x * (1 + i) + (CellSize.x / 2)-10, yPos,Palette::Blue);
			hitBoxs[i].x = CellSize.x * (1 + i);
			hitBoxs[i].y = yPos;
			//hitBoxs[i].stretched(-2).draw(ColorF{ 1.0, 0.6 });

			if (hitBoxs[i].mouseOver())
			{
				hitBoxs[i].stretched(-2).draw(ColorF{ 1.0, 0.6 });
			}
		}
	}
private:

};

