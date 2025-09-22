#include "util_testing.h"

#include "libcutils/util_makros.h"
#include "../code/game_x01.h"

struct Checkout_Testcases {
	const char *testname;
	enum X01_Mode checkout_mode;
	int player_score;
	struct Dart_Hit current_throw;
	enum X01_Result expected_result;
};
static void run_testcases(struct Checkout_Testcases *cases, size_t count)
{
	struct Player   player = {0};
	struct Game_X01 game   = {0};

	for (size_t i=0; i < count; ++i) {
		printf("TEST %02d of %02d: %s\n", (int)i+1, (int)count, cases[i].testname);

		game.check_out = cases[i].checkout_mode;
		player.score   = cases[i].player_score;
		player_set_dart_throw(&player, cases[i].current_throw);

		TEST_ASSERT_EQ_INT(
				cases[i].expected_result,
				game_x01_register_dart_throw(&game, &player)
		);

	}
}

static void test_finisher(void)
{
	struct Checkout_Testcases cases[] = {
		{
			.testname = "GIVEN_checkout_straight_WHEN_finish_single_THEN_player_won",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 10),
			.expected_result = X01_RESULT_PLAYER_WON
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_finish_double_THEN_player_won",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 5),
			.expected_result = X01_RESULT_PLAYER_WON
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_finish_tripple_THEN_player_won",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 15,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 5),
			.expected_result = X01_RESULT_PLAYER_WON
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_finish_single_THEN_checkout_not_satisfied",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 10),
			.expected_result = X01_RESULT_CHECKOUT_NOT_SATISFIED
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_finish_double_THEN_player_won",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 5),
			.expected_result = X01_RESULT_PLAYER_WON
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_finish_tripple_THEN_checkout_not_satisfied",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 15,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 5),
			.expected_result = X01_RESULT_CHECKOUT_NOT_SATISFIED
		},
		{
			.testname = "GIVEN_checkout_master_WHEN_finish_single_THEN_checkout_not_satisfied",
			.checkout_mode   = X01_MODE_MASTER,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 10),
			.expected_result = X01_RESULT_CHECKOUT_NOT_SATISFIED
		},
		{
			.testname = "GIVEN_checkout_master_WHEN_finish_double_THEN_player_won",
			.checkout_mode   = X01_MODE_MASTER,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 5),
			.expected_result = X01_RESULT_PLAYER_WON
		},
		{
			.testname = "GIVEN_checkout_master_WHEN_finish_tripple_THEN_player_won",
			.checkout_mode   = X01_MODE_MASTER,
			.player_score    = 15,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 5),
			.expected_result = X01_RESULT_PLAYER_WON
		},
	};

	run_testcases(cases, ARRAY_SIZE(cases));
}


static void test_remaining_points_when_checkout_straight(void)
{
	struct Checkout_Testcases cases[] = {
		{
			.testname = "GIVEN_checkout_straight_WHEN_one_point_left_WITH_single_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 9),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_one_point_left_WITH_double_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 9,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 4),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_one_point_left_WITH_tripple_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 3),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_two_points_left_WITH_single_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 8),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_two_points_left_WITH_double_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 4),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_two_points_left_WITH_tripple_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 11,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 3),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_three_points_left_WITH_single_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 7),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_three_points_left_WITH_double_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 11,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 4),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_straight_WHEN_three_points_left_WITH_tripple_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_STRAIGHT,
			.player_score    = 12,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 3),
			.expected_result = X01_RESULT_CONTINUE
		},
	};

	run_testcases(cases, ARRAY_SIZE(cases));
}

static void test_remaining_points_when_checkout_double(void)
{
	struct Checkout_Testcases cases[] = {
		{
			.testname = "GIVEN_checkout_double_WHEN_one_point_left_WITH_single_throw_THEN_player_overshoot",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 9),
			.expected_result = X01_RESULT_PLAYER_OVERSHOOT 
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_one_point_left_WITH_double_throw_THEN_player_overshoot",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 9,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 4),
			.expected_result = X01_RESULT_PLAYER_OVERSHOOT
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_one_point_left_WITH_tripple_throw_THEN_player_overshoot",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 3),
			.expected_result = X01_RESULT_PLAYER_OVERSHOOT
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_two_points_left_WITH_single_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 8),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_two_points_left_WITH_double_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 4),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_two_points_left_WITH_tripple_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 11,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 3),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_three_points_left_WITH_single_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 10,
			.current_throw   = dart_hit_make(DARTS_SINGLE, 7),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_three_points_left_WITH_double_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 11,
			.current_throw   = dart_hit_make(DARTS_DOUBLE, 4),
			.expected_result = X01_RESULT_CONTINUE
		},
		{
			.testname = "GIVEN_checkout_double_WHEN_three_points_left_WITH_tripple_throw_THEN_player_can_continue",
			.checkout_mode   = X01_MODE_DOUBLE,
			.player_score    = 12,
			.current_throw   = dart_hit_make(DARTS_TRIPPLE, 3),
			.expected_result = X01_RESULT_CONTINUE
		},
	};

	run_testcases(cases, ARRAY_SIZE(cases));
}

void main_test_game_x01(void)
{
	RUNTEST(test_finisher);
	RUNTEST(test_remaining_points_when_checkout_straight);
	RUNTEST(test_remaining_points_when_checkout_double);
}
