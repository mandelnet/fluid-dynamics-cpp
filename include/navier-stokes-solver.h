#ifndef __SHAPE_H__
#define __SHAPE_H__

class NavierStokesSolver{
    public:
        const static int N = 128.0;
        constexpr static double N_INVERSE = 1.0 / N;
        const static int SIZE = (N+2) * (N+2);

        double u[SIZE];
        double v[SIZE];
        double u_prev[SIZE];
        double v_prev[SIZE];
        double dense[SIZE];
        double dense_prev[SIZE];

        void clear();
        double getDx(int x, int y);
        double getDy(int x, int y);
        void applyForce(int cellX, int cellY, double vx, double vy);
        void tick(double dt, double visc, double diff);
        double* getInverseWarpPosition(double x, double y, double scale);
        double lerp(double x0, double x1, double l);
        int INDEX(int i, int j);
        void SWAP(double* x0, double* x);
        void add_source(double* x, double* s, double dt);
        void diffuse(int b, double* x, double* x0, double diff, double dt);
        void advect(int b, double* d, double* d0, double* u, double* v, double dt);
        void set_bnd(int b, double* x);
        void dens_step(double* x, double* x0, double* u, double* v, double diff, double dt);
        void vel_step(double* u, double* v, double* u0, double* v0, double visc, double dt);
        void project(double* u, double* v, double* p, double* div);
};

#endif
