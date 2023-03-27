CREATE DATABASE cars;

CREATE TABLE users(id serial PRIMARY KEY, username text unique not null, phone text not null,  password text not null, type int, first_name text, second_name text, avatar text, num_golds int default 20);

CREATE TABLE cars(id serial PRIMARY KEY, user_id int not null, make text, class text, model text, submodel int, country int, province int, sub_province int, price int, body_type int, year int, engine_type int, engine_size float, engine_power int, acc_0_to_100 float, transmission int, stearing_wheel int, exchange int, customs_cleared int, color int, on_sale int, on_top int, avatar text, mileage int, description text, avatar_image_id int, refresh_ts bigint, on_top_ts bigint);

CREATE TABLE car_images(id serial PRIMARY KEY, car_id int, image_path text);