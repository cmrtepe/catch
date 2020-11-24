#pragma once

#include "model.hxx"
#include <ge211.hxx>
#include <string>
extern ge211::Color const ball_color,
                          portal_color;


struct Ui : ge211::Abstract_game
{
    explicit Ui(Model&);


    ge211::Dimensions initial_window_dimensions() const override;


    void draw(ge211::Sprite_set&) override;

    // When w, a, s, d keys go down, updates
    // the catcher acceleration. If space bar
    // goes down when the game is over, restarts.
    void on_key_down(ge211::Key) override;

    // When w, a, s, d keys go up, updates
    // the catcher acceleration.
    void on_key_up(ge211::Key) override;

    // Runs on each frame, updates the model.
    void on_frame(double dt) override;

    // Model of the game.
    Model& model_;

    // Keeps track of whether the score was increased
    // on the last frame.
    bool scored_;

    // The sprite for the score.
    ge211::Text_sprite score_sprite_;

    // The sprite for the cather, which changes
    // depending on catcher_color.
    ge211::Rectangle_sprite catcher_sprite_;

    // The sprite for time.
    ge211::Text_sprite time_sprite;



    ge211::Circle_sprite    const
            ball_sprite_      {3, ball_color};


    ge211::Rectangle_sprite const
            portal_sprite_    {portal_dims, portal_color};


    ge211::Text_sprite      const
            game_over_sprite_ {"Press space to restart!",
                               {"sans.ttf", 25}};


};