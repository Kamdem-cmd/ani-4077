#include "Maillage.h"
#include "Gl.h"

namespace monrhi {

void Maillage::Creer(const Sommet *sommets, int nombreSommets) {
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Sommet) * (size_t)nombreSommets), sommets, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Sommet), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Sommet), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mNombre = nombreSommets;
    mEbo = 0;
}

void Maillage::CreerIndexe(const Sommet *sommets, int nombreSommets, const unsigned short *index, int nombreIndex) {
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Sommet) * (size_t)nombreSommets), sommets, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Sommet), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Sommet), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(sizeof(unsigned short) * (size_t)nombreIndex),
                 index, GL_STATIC_DRAW);

    // On délie le tableau d'abord, puis le buffer array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mNombre = nombreIndex;
}

void Maillage::Dessiner() const {
    glBindVertexArray(mVao);
    if (mEbo != 0) {
        glDrawElements(GL_TRIANGLES, (GLsizei)mNombre, GL_UNSIGNED_SHORT, (const void *)0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mNombre);
    }
    glBindVertexArray(0);
}

void Maillage::Detruire() {
    if (mVao != 0) { glDeleteVertexArrays(1, &mVao); mVao = 0; }
    if (mVbo != 0) { glDeleteBuffers(1, &mVbo); mVbo = 0; }
    if (mEbo != 0) { glDeleteBuffers(1, &mEbo); mEbo = 0; }
}
Maillage::~Maillage() {
    Detruire();
}

} // namespace monrhi