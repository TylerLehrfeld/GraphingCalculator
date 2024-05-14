
void destroyGraph(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
