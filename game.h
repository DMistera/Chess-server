class Game {
    public:
        Game(int white, int black);
        int getWhiteSocket();
        int getBlackSocket();
    private:
        int m_whiteSocket;
        int m_blackSocket;
};