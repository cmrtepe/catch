#pragma once

#include <ge211.hxx>



extern const double            ball_initial_speed;
extern const int               ball_radius,
                               key_acc,
                               b,
                               max_score;
extern const ge211::Dimensions catcher_dims,
                               scene_dims,
                               zone_1_dims,
                               portal_dims;

struct Portal
{
    ge211::Rectangle  rec()
    {
        return ge211::Rectangle{top_left.x, top_left.y,
                                dims.width, dims.height};
    }
    ge211::Position top_left;
    ge211::Dimensions dims;
    double angle;
};

class Model
{


public:
    //
    // Public functions
    //
    Model();

    // Returns the ball's position.
    ge211::Position get_ball() const;

    // Returns the catcher's position.
    ge211::Position get_catcher() const;

    // Returns the velocity of the ball.
    //ge211::Dimensions vel() const;

    //Returns the key part of the acceleration of
    //the catcher.
    void catcher_acc_update(ge211::Dimensions);

    // Updates the radius_ to the value x.
    void update_radius(int x);


    // Returns the velocity of the catcher.
    //ge211::Dimensions catcher_vel();
    std::vector<Portal> get_portals();




    // Returns the time elapsed since the start of the game.
    double time();

    // Returns the score.
    int score();

    // Returns two different colors depending on
    // whether the ball is inside the catcher or not.
    ge211::Color catcher_color();

    // Only returns true if ball is in zone 1 and its next position is
    // in zone 2
    bool enters_zone_2(double dt);

    // Updates radius_ with a random int value in some
    // range when enters_zone_2() is true. Updates the
    // model with either arch() or straight() helper functions.
    // upper limit might depend on the size of
    // the screen or just be something small
    // like 500. The second int value 1, 2, or 3.
    // determines the portal to teleport.
    // teleport(1) ,helper defined below, teleports
    // to portal_1 and so on.
    void update(int x, double dt);

    // Returns true if the max score is reached.
    bool game_over();

    // Resets the game.
    void reset();
    //
    // Variables supposed to be private, added to public just
    // for testing.
    // Rectangles that are considered zone 1. If not in zone 1
    // then in zone 2.
    std::vector<ge211::Rectangle> zone_1_;

    // Portals in the game.
    std::vector<struct Portal> portals_;

    // The top left of the square representing the catcher.
    ge211::Position catcher_;

    // The center of the ball.
    ge211::Basic_position<double> ball_;

    // The velocity of the ball.
    ge211::Basic_dimensions<double> vel_;

    // The velocity of the catcher.
    ge211::Dimensions catcher_vel_;

    // The acceleration of the catcher.The -b*vel part
    // will be inside the code and not included here.
    // Initialized as (0,0).
    ge211::Dimensions catcher_acc_;

    // The radius of the arch.
    int radius_;

    // The score.
    int score_;

    //The time since the start of the game.
    ge211::Timer time_;

    // The speed_, limits the magnitude of
    // the balls velocity.
    double speed_;

    // Determines whether the previous position
    // of the ball was inside the cather.
    bool prev_;

    // True if the game is over, meaning max score
    // is reached.
    bool game_over_;



private:


    // Returns the ball's zone either 1 or 2.
    int zone(ge211::Basic_position<double>) const;




    // Determines whether the ball hits a portal.
    bool ball_hits_portal(double dt);

    // Determines whether the ball hits the top or the bottom.
    bool ball_hits_vertical(double dt);
    // Determines whether the ball hits the sides.
    bool ball_hits_horizontal(double dt);

    bool catcher_hits_vertical(double dt);

    bool catcher_hits_horizontal(double dt);

    bool catcher_hits_portal(double dt);
    //Helper
    bool inside(ge211::Basic_position<double>, ge211::Rectangle) const;

    //Helper
    bool inside_portal(ge211::Basic_position<double>,
            ge211::Rectangle,
            double angle) const;

    // Speeds the ball up when the player reaches 10 points. Doesn't do
    // anything otherwise.
    void update_speed();

    void update_score(double dt);




    // Teleports the ball to the portal number x in {1,2,3}, in total there
    // are 3 portals. The location of the ball is the center of the portal x
    // with velocity perpendicular to the long edge of the portal with
    // a direction pointing away from the center.
    void teleport(int x);

    //Gives the position to teleport and the unit vector for the velocity
    //after the teleport, each portal( 1, 2, or 3) has its own constant
    //location and velocity is perpendicular to the long edge.
    std::pair<ge211::Basic_position<double>, ge211::Basic_dimensions<double>>
    portal_loc(int x);

    // Moves the ball in an arch with radius_.
    void arch(double dt);

    // Moves the ball in a straight line.
    void straight(double dt);

    // function for testing
    void update_vel(ge211::Basic_dimensions<double> vel)
    { vel_ = vel;}


    static double square(ge211::Basic_dimensions<double>);

    // Returns the unit vector perpendicular to the velocity in the direction
    // determined by the vector relation orthogonal = velocity x Z , where Z
    // is the unit vector in the direction coming out of the screen.
    ge211::Basic_dimensions<double>
    orthogonal(ge211::Basic_dimensions<double>);



    // Test access.
    friend struct Test_access;


};

