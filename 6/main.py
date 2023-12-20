import pygame
from OpenGL.GL import *
from OpenGL.GLU import *


vertices = (
    (0.1, -0.75, -0.5),
    (0.1, 0.25, -0.5),
    (-0.5, 0.25, -0.5),
    (-0.5, -0.75, -0.5),
    (0.1, -0.75, 0.5),
    (0.1, 0.25, 0.5),
    (-0.5, -0.75, 0.5),
    (-0.5, 0.25, 0.5),

    (0.5, 0.75, -0.5),
    (0.5, 0.75, 0.5),
    (-0.5, 0.75, -0.5),
    (-0.5, 0.75, 0.5),

    (0.85, 0.75, -0.5),
    (0.85, 0.75, 0.5),
    (0.85, 0.25, -0.5),
    (0.85, 0.25, 0.5)
)


edges = (
    (0, 1),
    (2, 3),
    (3, 0),
    (6, 7),
    (6, 3),
    (6, 4),
    (4, 5),
    (0, 4),

    (2, 10),
    (11, 7),
    (11, 9),
    (10, 8),
    (11, 10),

    (8, 12),
    (9, 13),
    (1, 14),
    (5, 15),
    (13, 15),
    (12, 14),
    (14, 15),
    (12, 13)
)


rotation_speed = 15.0


def draw_letter():
    glLineWidth(4)
    glBegin(GL_LINES)
    for edge in edges:
        for vertex in edge:
            glColor3fv((0, 0, 1))
            glVertex3fv(vertices[vertex])
    glEnd()


def main():

    pygame.init()
    display = (800, 600)
    pygame.display.set_mode(display, pygame.DOUBLEBUF | pygame.OPENGL)
    pygame.display.set_caption("PyOpenGL")
    gluPerspective(45, (display[0] / display[1]), 0.1, 50.0)
    glTranslatef(0.0, 0.0, -5)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    glRotatef(rotation_speed, 0, 2, 0)
                elif event.key == pygame.K_RIGHT:
                    glRotatef(-rotation_speed, 0, 2, 0)
                elif event.key == pygame.K_UP:
                    glRotatef(rotation_speed, 2, 0, 0)
                elif event.key == pygame.K_DOWN:
                    glRotatef(-rotation_speed, 2, 0, 0)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        draw_letter()
        pygame.display.flip()
        pygame.time.wait(10)


if __name__ == "__main__":
    main()
