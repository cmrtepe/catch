
#include "ui.hxx"

ge211::Color const ball_color    {255,50,175},
        catcher_color {50, 50, 175},
        portal_color  {255,50,175};




Ui::Ui(Model& model)
        : model_(model),
        scored_(false),
        score_sprite_(std::to_string(model_.score()),
                {"sans.ttf",30}),
        catcher_sprite_{catcher_dims, model_.catcher_color()},
        time_sprite{std::to_string((int) model_.time()).append
        (".").append(std::to_string(00)),
        {"sans.ttf",30}}
{ }



ge211::Dimensions Ui::initial_window_dimensions() const
{
    return scene_dims;
}

void Ui::draw(ge211::Sprite_set& sprites)
{
    sprites.add_sprite(ball_sprite_, model_.get_ball(), 3);
    sprites.add_sprite(catcher_sprite_, model_.get_catcher(), 2);

    for (int i = 0; i < 3; i++)
    {
        sprites.add_sprite(portal_sprite_,model_.get_portals()[i].top_left,
                           1,
                           ge211::Transform::rotation
                           (model_.get_portals()[i].angle));
    }
    sprites.add_sprite(score_sprite_, {scene_dims.width/2,50},1);
    if (model_.game_over())
    {
        sprites.add_sprite(game_over_sprite_,{50,50},4);
    }
    sprites.add_sprite(time_sprite, {scene_dims.width - 200,  50}, 1);
}

void Ui::on_key_down(ge211::Key key)
{
    if (model_.game_over())
    {
        if (key == ge211::Key::code(' '))
        {
            model_.reset();
        }
    }
    if (key == ge211::Key::code('w'))
    {
        model_.catcher_acc_update({0,-key_acc});
    }
    if (key == ge211::Key::code('s'))
    {
        model_.catcher_acc_update({0,key_acc});
    }
    if (key == ge211::Key::code('a'))
    {
        model_.catcher_acc_update({-key_acc,0});
    }
    if (key == ge211::Key::code('d'))
    {
        model_.catcher_acc_update({key_acc,0});
    }
}




void Ui::on_key_up(ge211::Key key)
{
    if (key == ge211::Key::code('w'))
    {
        model_.catcher_acc_update({0,key_acc});
    }
    if (key == ge211::Key::code('s'))
    {
        model_.catcher_acc_update({0,-key_acc});
    }
    if (key == ge211::Key::code('a'))
    {
        model_.catcher_acc_update({key_acc,0});
    }
    if (key == ge211::Key::code('d'))
    {
        model_.catcher_acc_update({-key_acc,0});
    }

}

void Ui::on_frame(double dt)
{
    int x = model_.score();
    score_sprite_ = ge211::Text_sprite {std::to_string( x ),
                                        {"sans.ttf", 30}};

    if (!model_.game_over())
    {
        time_sprite   = ge211::Text_sprite
                {std::to_string((int) model_.time()).append(".")
                 .append(std::to_string
                 ((int) (100*model_.time()) -
                              100*((int) model_.time()))),
                {"sans.ttf",30}};
        catcher_sprite_ =
                ge211::Rectangle_sprite{catcher_dims, model_.catcher_color()};
        if (model_.enters_zone_2(dt)) {
            model_.update_radius(get_random().between(50, 80));
        }
        model_.update(get_random().between(1, 3), dt);
        if (x != model_.score()) {
            score_sprite_ = ge211::Text_sprite
                    {std::to_string( model_.score() ),
                     {"sans.ttf", 30}};
            scored_ = true;
        }
    }
}