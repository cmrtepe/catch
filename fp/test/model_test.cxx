#include "model.hxx"
#include <catch.hxx>
#include <model.hxx>

using namespace ge211;

struct Test_access
{
    Model& model;


    void arch(double dt)
    { return model.arch(dt); }

    Basic_dimensions<double> get_vel()
    { return model.vel_; }

    Basic_dimensions<double> orthogonal(Basic_dimensions<double> vec)
    { return model.orthogonal(vec); }

    bool inside(Basic_position<double> pos, Rectangle rec)
    { return model.inside(pos, rec); }

    bool inside_portal(Basic_position<double> pos,
                        Rectangle rec, double angle)
    { return model.inside_portal(pos, rec, angle); }

    bool ball_hits_vertical(double dt)
    { return model.ball_hits_vertical(dt); }

    bool ball_hits_horizontal(double dt)
    { return model.ball_hits_horizontal(dt);}

    bool ball_hits_portal(double dt)
    { return model.ball_hits_portal(dt); }

    bool catcher_hits_portal(double dt)
    { return model.catcher_hits_portal(dt); }

    bool catcher_hits_vertical(double dt)
    { return model.catcher_hits_vertical(dt); }

    bool catcher_hits_horizontal(double dt)
    { return model.catcher_hits_horizontal(dt); }


};
TEST_CASE("Arch")
{
    Model m;
    Test_access t{m};
    CHECK((t.get_vel().width == 0 && t.get_vel().height == 300)); //initial vel
    CHECK((t.orthogonal(t.get_vel()).width == -1 &&
           t.orthogonal(t.get_vel()).height == 0));
    m.update_radius(100);
    t.arch(1);
    Basic_dimensions<double> new_vel = {-900,300};
    CHECK((new_vel.height == t.get_vel().height &&
           new_vel.width  == t.get_vel().width));

}

TEST_CASE("Position inside")
{
    Model m;
    Test_access t{m};

    CHECK(t.inside_portal({15,15}, {-25,-10,50,20},45));
    CHECK(!t.inside({5,5},{5,5,50,50}));
    CHECK(t.inside({5.0001,5.00001},{5,5,50,50}));
}

TEST_CASE("Ball hits stuff")
{
    Model model;
    Test_access t{model};
    model.ball_ = {5,6};
    model.vel_  = {-10,-10};
    CHECK((!t.ball_hits_vertical(1) && t.ball_hits_horizontal(1)));
    model.portals_[0] = Portal{{10,0},20,12,45};
    model.vel_ = {15, 0};
    CHECK(model.portals_[0].angle == 45);
    CHECK(t.inside_portal({20, 6}, model.portals_[0].rec(),
            model.portals_[0].angle));
    CHECK(t.ball_hits_portal(1));
}

TEST_CASE("Catcher hits stuff")
{
    Model model;
    Test_access t{model};
    model.catcher_ = {50,50};
    model.catcher_vel_ = {25,25};
    model.portals_[0] = Portal{{100,100},10,10,25};
    CHECK(t.catcher_hits_portal(1));
    model.catcher_vel_ = {20,-70};
    CHECK(t.catcher_hits_vertical(1));
    model.catcher_vel_ = {-70,20};
    CHECK(t.catcher_hits_horizontal(1));
}

TEST_CASE("Update")
{
    Model model;
    model.catcher_ = {90,90};
    model.ball_ = {80,100};
    model.catcher_vel_ = {0,0};
    model.vel_ = {20,0};
    model.score_ = 0;
    CHECK(model.speed_ == ball_initial_speed);
    model.update(1,1); // ball enters catcher
    CHECK(model.score_ == 1);
    model.update(1,1); // ball still inside catcher
    CHECK(model.score_ == 1);
    model.ball_ = {80,100};
    model.score_ = max_score/2 -1;
    model.update(1,1);
    CHECK(model.speed_ == 1.5*ball_initial_speed);
}

