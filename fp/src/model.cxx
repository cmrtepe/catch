#include "model.hxx"
#include <cmath>
const ge211::Dimensions scene_dims{1024,768};

const ge211::Dimensions catcher_dims{30, 30},
                        portal_dims{120,20},
                        zone_1_dims{40,40};
const int               b{2},
                        key_acc{1000},
                        max_score{4},
                        zone_spacing{80},
                        zone_start{25};
const double            ball_initial_speed{300};
Model::Model()
        : catcher_{40, scene_dims.height- 40}// right bottom corner
        , ball_{(double) scene_dims.width/2, (double) scene_dims.height/2}
        , vel_{ 0 , ball_initial_speed}
        , catcher_vel_{0, 0}
        , catcher_acc_{0,0}
        , radius_{50}
        , score_{0}
        , time_{ge211::Timer()}
        , speed_{ball_initial_speed}
        , prev_{false}
        , game_over_(false)
{
    ge211::Position portal_2 = {scene_dims.width/2 - portal_dims.width/2,
                                scene_dims.height/2 + portal_dims.width};
    ge211::Position portal_3 = {scene_dims.width/2 -  2*portal_dims.width,
                                scene_dims.height/2 - portal_dims.width};
    ge211::Position portal_1 = {scene_dims.width/2 + portal_dims.width,
                                scene_dims.height/2 - portal_dims.width};
    portals_.push_back(Portal{portal_3, portal_dims, -60});
    portals_.push_back(Portal{portal_2, portal_dims, 180});
    portals_.push_back(Portal{portal_1, portal_dims, 60});
    for (int i = 0; i < (int) scene_dims.height/zone_1_dims.height; ++i)
    {
        for (int j = 0; j < (int) scene_dims.width/zone_1_dims.width ; ++j)
        {
            zone_1_.push_back(ge211::Rectangle
            {j*zone_start + i*(zone_1_dims.height + zone_spacing),
             j*(zone_1_dims.width + zone_spacing),
             zone_1_dims.width,
             zone_1_dims.height});
        }
    }

}

ge211::Position Model::get_ball() const { return ball_.into<int>();}





void Model::update_radius(int x)
{
    radius_ = x;
}
ge211::Position Model::get_catcher() const
{
    return catcher_;
}
void Model::catcher_acc_update(ge211::Dimensions acc)
{
    catcher_acc_.width += acc.width;
    catcher_acc_.height += acc.height;
}

//ge211::Dimensions Model::catcher_vel() { return catcher_vel_;}

std::vector<Portal> Model::get_portals() { return portals_;}

double Model::time() { return time_.elapsed_time().seconds();}

int Model::score() { return score_;}

ge211::Color Model::catcher_color()
{
    ge211::Rectangle rec{catcher_.x,catcher_.y,
                         catcher_dims.width,catcher_dims.height};
    if (inside(ball_, rec)) return {0,255,50};
    else                         return {50, 50, 175};
}

void Model::reset()
{
    *this = Model();
}

void Model::update(int x, double dt)
{
    prev_ = inside(ball_,ge211::Rectangle{catcher_.x,
                                          catcher_.y,
                                          catcher_dims.width,
                                          catcher_dims.height});

    if ( score_ >= max_score) {game_over_ = true;}
    else{
        if (catcher_hits_vertical(dt))
        {
            catcher_vel_.height *= -1;
        }
        if (catcher_hits_horizontal(dt))
        {
            catcher_vel_.width *= -1;
        }
        if (catcher_hits_portal(dt))
        {
            catcher_vel_ *= -1;
        }
        catcher_ += catcher_vel_*dt;
        catcher_vel_ += dt*(-b*catcher_vel_ + catcher_acc_);
        if ( ball_hits_horizontal(dt))
        {
            vel_.width *= -1;
        }
        if (ball_hits_vertical(dt))
        {
            vel_.height *= -1;
        }
        if (ball_hits_portal(dt))
        {
            teleport(x);
        }
        if (square(vel_) > 1.2*(speed_*speed_))
        {
            vel_ = speed_/sqrt(square(vel_))*vel_;
        }
        if ( zone(ball_) == 1 )
        {
            arch(dt);
        }
        else{
            straight(dt);
        }
        update_score(dt);
    }
}

void Model::straight(double dt)
{
    ball_ += vel_*dt;
}

double Model::square(ge211::Basic_dimensions<double> vec)
{
    return vec.width*vec.width + vec.height*vec.height;
}

void Model::arch(double dt)
{
    ball_ += vel_*dt;
    vel_ +=dt*square(vel_)/radius_*orthogonal(vel_);
}

ge211::Basic_dimensions<double>
Model::orthogonal(ge211::Basic_dimensions<double> vec)
{;
    return {(-1/sqrt(square(vec)))*vec.height,
            (1/sqrt(square(vec)))*vec.width};
}

void Model::teleport(int x)
{
    ball_ = {portal_loc(x).first.x, portal_loc(x).first.y};
    vel_ = {portal_loc(x).second.width,
            portal_loc(x).second.height};
}

std::pair<ge211::Basic_position<double>, ge211::Basic_dimensions<double>>
Model::portal_loc(int x)
{
    ge211::Basic_dimensions<double> exit = {0, 0};
    ge211::Basic_position<double> pos={0,0};
    if (x < portals_.size() + 1)
    {
        double pi = 3.14159265;
        exit = {-cos(pi/2 + portals_[x-1].angle*pi/180)*speed_,
                -sin(pi/2 + portals_[x-1].angle*pi/180)*speed_};
        pos = {portals_[x-1].top_left.into<double>().x +
               portals_[x-1].dims.into<double>().width/2 -
               4*portals_[x-1].dims.height*
               cos(pi/2 + portals_[x-1].angle*pi/180),
               portals_[x-1].top_left.into<double>().y -
               portals_[x-1].dims.height/2 -
               4*portals_[x-1].dims.height*
               sin(pi/2 + portals_[x-1].angle*pi/180)};
        return {pos,exit};
    }
    else return { pos, exit };



}

bool Model::inside(ge211::Basic_position<double> pos,
                   ge211::Rectangle rec) const
{
    return (pos.x < rec.x + rec.width && pos.x > rec.x
            && pos.y < rec.y + rec.height && pos.y > rec.y);
}

bool Model::inside_portal(ge211::Basic_position<double> pos,
                          ge211::Rectangle rec,
                          double angle) const
{
    if (angle == 0) return inside( pos, rec);
    else{
        double y =  pos.y - rec.y - rec.height/2 ;
        double x =  pos.x - rec.width/2 - rec.x ;
        double rad = angle*3.14159265/180;
        double new_x = x*cos(rad) + y*sin(rad);
        double new_y = -x*sin(rad) + y*cos(rad);
        ge211::Basic_position<double> new_pos = {new_x,new_y};
        ge211::Rectangle new_rec =
                {-rec.width/2,-rec.height/2,rec.width,rec.height};
        return inside(new_pos, new_rec);

    }

}

int Model::zone(ge211::Basic_position<double> pos) const
{
    for (ge211::Rectangle rec : zone_1_)
    {
        if (inside(pos, rec)) return 1;
    }
    return 2;
}

bool Model::enters_zone_2(double dt)
{
    ge211::Basic_position<double> next = ball_ + vel_*dt;
    return (zone(ball_) == 1 &&
        zone (next) == 2);
}

void Model::update_score(double dt)
{
    ge211::Rectangle rec{catcher_.x,catcher_.y,
                         catcher_dims.width,catcher_dims.height};
    ge211::Basic_position<double> next = ball_ + vel_*dt;
    ge211::Rectangle next_rec{(int) (catcher_.x + catcher_vel_.width*dt),
                              (int) (catcher_.y + catcher_vel_.height*dt),
                              catcher_dims.width,catcher_dims.height};
    if ((inside(ball_, rec) && !prev_))
    {
        ++score_;
        update_speed();
    }
}

bool Model::ball_hits_vertical(double dt)
{
    ge211::Position next = (ball_ + vel_*dt).into<int>();
    return (next.y < 0 || next.y > scene_dims.height);
}
bool Model::catcher_hits_vertical(double dt)
{
    ge211::Position next = catcher_vel_*dt + catcher_;
    return ( next.y < 0 || next.y + catcher_dims.height > scene_dims.height);
}

bool Model::ball_hits_horizontal(double dt)
{
    ge211::Position next = (ball_ + vel_*dt).into<int>();
    return (next.x < 0 || next.x > scene_dims.width);
}
bool Model::catcher_hits_horizontal(double dt)
{
    ge211::Position next = catcher_vel_*dt + catcher_;
    return ( next.x < 0 || next.x + catcher_dims.width > scene_dims.width);
}

void Model::update_speed()
{
    if (score_ == max_score/2)
    {
        speed_ *= 1.5;
        vel_.width *= sqrt(1.5);
        vel_.height *= sqrt(1.5);
    }
}

bool Model::ball_hits_portal(double dt)
{
    ge211::Basic_position<double> next = ball_ + vel_*dt ;
    for (int i =0; i < portals_.size(); i++)
    {
        if (inside_portal(next, portals_[i].rec(), portals_[i].angle))
            return true;
    }
    return false;
}
bool Model::catcher_hits_portal(double dt)
{
    ge211::Basic_position<double> next =
            (catcher_vel_*dt + catcher_).into<double>();
    std::vector<std::pair<double,double>> catcher_edge;
    for (int i = 0; i <= 10; i++)
    {
        std::pair<double, double> top =
                {next.x + i*catcher_dims.width/10, next.y};
        std::pair<double, double> bottom =
                {next.x + i*catcher_dims.width/10,
                                            next.y + catcher_dims.height};
        std::pair<double, double> left =
                {next.x,next.y + i*catcher_dims.height/10};
        std::pair<double, double> right =
                {next.x + catcher_dims.width,
                 next.y + i*catcher_dims.height/10};
        catcher_edge.push_back(top) ;
        catcher_edge.push_back(bottom);
        catcher_edge.push_back(left);
        catcher_edge.push_back(right);

    }
    for (Portal portal : portals_)
    {
        for (std::pair<double,double> fake_pos : catcher_edge)
        {
            ge211::Basic_position<double> pos =
                    {fake_pos.first,fake_pos.second};
            if (inside_portal(pos, portal.rec(), portal.angle)) return true;
        }
    }
    return false;

}
bool Model::game_over()
{
    return game_over_;
}
