package utils

import (
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/sdl"
)

func LoadTexture(path string, renderer *sdl.Renderer) *sdl.Texture {
	surface, err := img.Load(path)
	if err != nil {
		panic(err)
	}
	defer surface.Free()

	texture, err := renderer.CreateTextureFromSurface(surface)
	if err != nil {
		panic(err)
	}

	return texture
}

func RenderTexture(texture *sdl.Texture, renderer *sdl.Renderer, dest sdl.Rect) {
	renderer.Copy(texture, nil, &dest)
}
