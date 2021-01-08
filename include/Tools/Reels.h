#ifndef REELS_H
#define REELS_H


class Reels
{
public:
constexpr static /*const*/ double PI = 3.1415926;

/**
 *
 * RESULTAT : un nombre aléatoire x tel que 0 <= x < 1
 *
 * suppose qu'un appel à srand a été fait au préalable
 *
 * */
inline static double random();

/**
 *
 * RESULTAT : un nombre aléatoire x tel que | x - germe | < eps
 *
 * suppose qu'un appel à srand a été fait au préalable
 *
 * */
inline static double random(const double & germe, const double & eps);

/**
 *
 * RESULTAT : un nombre aléatoire x tel que | x - germe | < eps
 *
 * suppose qu'un appel à srand a été fait au préalable
 *
 * Pour être placée comme pointeur de fct paramètre d'une fct template
 *
 * */
inline static const double random1(const double & germe, const double & eps) {return Reels::random(germe,eps);}
};


/**
 *
 * RESULTAT : un nombre aléatoire x tel que 0 <= x < 1
 *
 * suppose qu'un appel à srand a été fait au préalable
 *
 * */
inline /* static */ double Reels::random()
{
return rand()/(double)RAND_MAX;
}


/**
 *
 * RESULTAT : un nombre aléatoire x tel que | x - germe | < eps
 *
 * suppose qu'un appel à srand a été fait au préalable
 * */
/* static */ inline double Reels::random(const double & germe, const double & eps)
{
double t = Reels::random(); // 0 <= t <= 1

return eps*( 2*t-1 ) + germe;
}



#endif // REELS_H