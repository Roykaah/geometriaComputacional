#include <iostream>
#include <vector>
#include <GL/glut.h>

struct Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};

struct Vertex {
    Point point;
    struct HalfEdge* incidentEdge;
    Vertex(double x, double y) : point(x, y), incidentEdge(nullptr) {}
};

struct HalfEdge {
    Vertex* origin;
    HalfEdge* twin;
    HalfEdge* next;
    HalfEdge* prev;
    struct Face* incidentFace;
    HalfEdge(Vertex* origin) : origin(origin), twin(nullptr), next(nullptr), prev(nullptr), incidentFace(nullptr) {}
};

struct Face {
    HalfEdge* outerComponent;
    Face() : outerComponent(nullptr) {}
};

struct DCEL {
    std::vector<Vertex*> vertices;
    std::vector<HalfEdge*> edges;
};

DCEL dcel;

HalfEdge* get_edge_by_origin(Vertex* a) {
    HalfEdge* he = nullptr;

    for (HalfEdge* e : dcel.edges) {
        if (e->origin == a) {
            he = e;
            break;
        }
    }

    return he;
}

HalfEdge* get_possible_prev_edge(HalfEdge* next) {
    HalfEdge* he = nullptr;

    for (HalfEdge* e : dcel.edges) {
        if (e->twin->origin == next->origin) {
            he = e->twin;
            break;
        }
    }

    return he;
}

HalfEdge* create_edge(Vertex* v1, Vertex* v2) {
    HalfEdge* e = new HalfEdge(v1);
    HalfEdge* et = new HalfEdge(v2);

    e->twin = et;
    et->twin = e;

    dcel.edges.push_back(e);

    return e;
}

Vertex* create_vertex(double x, double y) {
    Vertex* v = new Vertex(x, y);
    dcel.vertices.push_back(v);
    return v;
}

void connect_orbit(HalfEdge* e1, HalfEdge* e2) {
    e1->next = e2;
    e2->prev = e1;

    e2->twin->next = e1->twin;
    e1->twin->prev = e2->twin;
}

void connect(Vertex* a, Vertex* b) {
    HalfEdge* e3 = create_edge(a, b);

    HalfEdge* e1 = get_possible_prev_edge(e3);
    HalfEdge* e2 = get_edge_by_origin(b)->twin;

    connect_orbit(e1, e3);
    connect_orbit(e2, e3->twin);
}

void remove_if_disconnected(HalfEdge* a) {
    if (a->next == nullptr && a->prev == nullptr) {
        std::vector<HalfEdge*> edges;

        for (HalfEdge* e : dcel.edges) {
            if (e != a && e->twin != a) {
                edges.push_back(e);
            }
        }

        dcel.edges = edges;
    }
}

void disconnect_orbit(HalfEdge* a, HalfEdge* b) {
    a->next = a->prev;
    a->twin->prev = a->twin->next;
    // a->next = nullptr;
    // a->twin->prev = nullptr;

    b->prev = nullptr;
    b->twin->next = nullptr;

    remove_if_disconnected(a);
    remove_if_disconnected(b);
}

HalfEdge* get_edge_by_vertices(Vertex* a, Vertex* b) {
    HalfEdge* he = nullptr;

    for (HalfEdge* e : dcel.edges) {
        if (e->origin == a && e->twin->origin == b) {
            he = e;
            break;
        }
    }

    return he;
}

void disconnect(Vertex* a, Vertex* b) {
    HalfEdge* e3 = get_edge_by_vertices(a, b);
    HalfEdge* e1 = e3->prev;
    HalfEdge* e2 = e3->next->twin;

    disconnect_orbit(e1, e3);
    disconnect_orbit(e2, e3->twin);
}

void printAresta(HalfEdge* e) {
    if (e != nullptr){
        std::cout << "Origem: (" << e->origin->point.x << ", " << e->origin->point.y << ")" << std::endl;
    } else {
        std::cout << "Nulo" << std::endl;
    }
}

void printDCELInfo() {
    for (HalfEdge* e : dcel.edges) {
        std::cout << "Aresta:" << std::endl;
        printAresta(e);
        std::cout << "Twin:" << std::endl;
        printAresta(e->twin);
        std::cout << "Aresta Twin Twin:" << std::endl;
        printAresta(e->twin->twin);
        std::cout << "Aresta next:" << std::endl;
        printAresta(e->next);
        std::cout << "Aresta prev:" << std::endl;
        printAresta(e->prev);
        std::cout << "Twin next:" << std::endl;
        printAresta(e->twin->next);
        std::cout << "Twin prev:" << std::endl;
        printAresta(e->twin->prev);
        std::cout << "--------------------" << std::endl;
    }
}

void initializeDCEL() {
    Vertex* a = create_vertex(0.0, 0.0);
    Vertex* b = create_vertex(0.8, -0.1);
    Vertex* c = create_vertex(0.8, 0.8);
    Vertex* d = create_vertex(-0.3, 0.8);
    Vertex* e = create_vertex(-0.8, -0.1);
    Vertex* f = create_vertex(-0.8, -0.8);
    Vertex* g = create_vertex(0.1, -0.3);

    HalfEdge* e1 = create_edge(b, c);
    HalfEdge* e2 = create_edge(c, d);
    HalfEdge* e3 = create_edge(d, e);
    HalfEdge* e4 = create_edge(e, f);
    HalfEdge* e5 = create_edge(f, b);
    HalfEdge* e6 = create_edge(f, a);

    connect_orbit(e1, e2);
    connect_orbit(e2, e3);
    connect_orbit(e3, e4);
    connect_orbit(e4, e6);
    connect_orbit(e6, e5);
    connect_orbit(e5, e1);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            connect(dcel.vertices[0], dcel.vertices[2]);
            break;
        case 'r':
            disconnect(dcel.vertices[0], dcel.vertices[2]);
            break;
    }
    glutPostRedisplay();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); 

    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (Vertex* v : dcel.vertices) {
        glVertex2f(v->point.x, v->point.y);
    }
    
    glEnd();

    glBegin(GL_LINES);
    for (HalfEdge* e : dcel.edges) {
        glVertex2f(e->origin->point.x, e->origin->point.y);
        glVertex2f(e->twin->origin->point.x, e->twin->origin->point.y);
    }
    glEnd();

    printDCELInfo();
    
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("DCEL Display");
    
    initializeDCEL();
    
    glutDisplayFunc(display);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();

    for (Vertex* v : dcel.vertices)
        delete v;
    for (HalfEdge* e : dcel.edges){
        delete e->twin;
        delete e;
    }
    
    return 0;
}


