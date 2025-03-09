package ecs

import (
	"app/utils"
	"github.com/veandco/go-sdl2/sdl"
)

type SpriteComponent struct {
	Texture *sdl.Texture
	Width   int
	Height  int
	ZOrder  int
}

func (ecs *ECSManager) AddSpriteComponent(entity EntityID, spritePath string, width, height, zOrder int, renderer *sdl.Renderer) {
	ecs.Components.Sprites[entity] = SpriteComponent{
		Texture: utils.LoadTexture(spritePath, renderer),
		Width:   width,
		Height:  height,
		ZOrder:  zOrder,
	}
	ecs.Components.SpriteMask[entity] = true
}
