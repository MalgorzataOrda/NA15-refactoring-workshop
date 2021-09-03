#pragma once

#include <list>
#include <memory>
#include <functional>
#include <stdexcept>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"

class Event;
class IPort;

namespace Snake
{
struct ConfigurationError : std::logic_error
{
    ConfigurationError();
};

struct UnexpectedEventException : std::runtime_error
{
    UnexpectedEventException();
};


class Segments  //szkic nieuzywanej klasy
{

    struct Segment          
    {
        int x;
        int y;
    };

    std::list<Segment> m_segments;

    bool isSegmentAtPosition(int x, int y) const;          
    Segment calculateNewHead() const;
    void updateSegmentsIfSuccessfullMove(Segment const& newHead);
    void addHeadSegment(Segment const& newHead);
    void removeTailSegmentIfNotScored(Segment const& newHead);
    void removeTailSegment();
};

class Controller : public IEventHandler
{
public:
    Controller(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    Controller(Controller const& p_rhs) = delete;
    Controller& operator=(Controller const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;

private:
    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;

    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

    struct Segment          // to segments
    {
        int x;
        int y;
    };

    std::list<Segment> m_segments;  //to segments
    Direction m_currentDirection;

    void handleTimeoutInd();                            //to co w tym akapicie zostaje w controllerze
    void handleDirectionInd(std::unique_ptr<Event>);
    void handleFoodInd(std::unique_ptr<Event>);
    void handleFoodResp(std::unique_ptr<Event>);
    void handlePauseInd(std::unique_ptr<Event>);

    bool isSegmentAtPosition(int x, int y) const;          //to co w tym akapicie do segments
    Segment calculateNewHead() const;
    void updateSegmentsIfSuccessfullMove(Segment const& newHead);
    void addHeadSegment(Segment const& newHead);
    void removeTailSegmentIfNotScored(Segment const& newHead);
    void removeTailSegment();

    bool isPositionOutsideMap(int x, int y) const; // zostaje w controllerze

    void updateFoodPosition(int x, int y, std::function<void()> clearPolicy);  //do world
    void sendClearOldFood();
    void sendPlaceNewFood(int x, int y);

    bool m_paused;
};

} // namespace Snake
