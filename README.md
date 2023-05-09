# DuelBot

A discord bot made for playing two player games, it can be invited with this link:

[DuelBot invite link](https://discord.com/api/oauth2/authorize?client_id=1053701904787570820&permissions=28032446758000&scope=bot)

Get support or just chat in:

[Bot Showcase](https://discord.gg/KaFpXjTH)

Or the [D++](https://dpp.dev/index.html) discord server:

[D++ - The C++ Discord API Library](https://discord.gg/dpp)

If you are a developer curious in building DuelBot, visit `BUILDING.md`

## Commands:

`info`: Provides a short description of DuelBot and links to this repository

`challenge <game> <player> [<ranked?>]`: Challenge `player` to `game`, by default, a game is played ranked, however it can be unranked if `ranked` is set to false. This will pop up a modal, with three input lines, the start time, the increment time, and the grace time, which, in chess time notation, is `start (m)|increment (s)|grace (s)`. The receiving user can choose to either accept or reject this game

`getrate <game> [<player>]`: Get's a given player's (by default sender of command) rating in `game`, if they allow others to see their rating.

`changesetting <setting> <value> [<gametype>][<effectglobal>]`: Change user option `setting` to `value` for `gametype`. `effectglobal`, if set, can be used to change where it is changed, where global is the default and any server specific setting has higher precedence 

`joinqueue <game> [<time>]`: Join a queue for the game `game`  with time control `time`, always ranked

## For Mathematical and rating curiosity:

I developed a formula for the rating bias, and it goes as follows:

$$
\frac{r_1-\sqrt{r_1r_2}}{a\ast(r_1r_2)}
$$

Where $r_1$ is your rating, $r_2$ is your opponents rating, and $a$ is a constant, which serves as a limiting factor as a equation, so say you gain $b$ points for winning, $0$ for drawing, and $-b$ for drawing by default. What $a$ does, is it limits how far your final point gain/loss can differ from the average. If we write our default rule from before as $(-b,0,b)$, we can say the worst case scenario (where we lose the most points for losing and win the least for winning) is:

$$
\left(-b-\frac{b}{a},-\frac{b}{a},b-\frac{b}{a}\right)
$$

And the best case scenario is:

$$
\left(-b+\frac{b}{a},\frac{b}{a},b+\frac{b}{a}\right)
$$

By default, in DuelBot, it `b=8` and `a=2`, therefore the above become:

$$
(-12,-4,4)
$$

And

$$
(-4,4,12)
$$

### The queue system:

**TODO**
