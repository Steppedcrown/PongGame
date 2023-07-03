#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_p, player_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 1;

int player_1_score, player_2_score;

internal void
simulate_player(float* p, float* dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * 0.5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal void
simulate_game (Input* input, float dt) {
	clear_screen(0xff5500);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);

	float player_ddp = 0.f;
	if (is_down(BUTTON_UP)) player_ddp += 1750;
	if (is_down(BUTTON_DOWN)) player_ddp -= 1750;

	float player_2_ddp = 0.f;
#if 0
	if (is_down(BUTTON_W)) player_2_ddp += 1750;
	if (is_down(BUTTON_S)) player_2_ddp -= 1750;
#else
	if (ball_p_y > player_2_p) player_2_ddp += 1750;
	if (ball_p_y < player_2_p) player_2_ddp -= 1750;
#endif
	
	simulate_player(&player_p, &player_dp, player_ddp, dt);
	simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

	//Simulate Ball
	{
		ball_p_x += ball_dp_x * dt;
		ball_p_y += ball_dp_y * dt;

		if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
			ball_p_x - ball_half_size < 80 + player_half_size_x &&
			ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
			ball_p_y + ball_half_size < player_2_p + player_half_size_y) {
			ball_p_x = 80 - player_half_size_x - ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_dp_y - player_2_p) * 1.5f;
		}
		else if (ball_p_x + ball_half_size > -80 - player_half_size_x &&
			ball_p_x - ball_half_size < -80 + player_half_size_x &&
			ball_p_y + ball_half_size > player_p - player_half_size_y &&
			ball_p_y + ball_half_size < player_p + player_half_size_y) {
			ball_p_x = -80 + player_half_size_x + ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_dp_y - player_p) * 1.5f;
		}

		if (ball_p_y + ball_half_size > arena_half_size_y) {
			ball_p_y = arena_half_size_y - ball_half_size;
			ball_dp_y *= -0.75; //-1 was too fast
		}
		else if (ball_p_y - ball_half_size < -arena_half_size_y) {
			ball_p_y = -arena_half_size_y + ball_half_size;
			ball_dp_y *= -0.75; //-1 was too fast
		}

		if (ball_p_x + ball_half_size > arena_half_size_x) {
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			player_1_score++;
		}
		if (ball_p_x - ball_half_size < -arena_half_size_x) {
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			player_2_score++;
		}
	}

	draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
	draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);

	draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);

	draw_rect(-80, player_p, player_half_size_x, player_half_size_y, 0x00ff22);
	draw_rect(80, player_2_p, player_half_size_x, player_half_size_y, 0x00ff22);
}