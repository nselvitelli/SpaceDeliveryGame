// ====================================================
#version 330 core

// ======================= uniform ====================
uniform vec3 u_color;

// ======================= IN =========================

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;



void main()
{
    // FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    FragColor = vec4(u_color, 1.0);
}
// ==================================================================
