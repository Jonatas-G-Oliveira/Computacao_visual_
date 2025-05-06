import cv2

# -- CHROMA
#  Imagem HSV -> color picker
#  Mascara binaria
#  Intervalo de matiz
# Aplicando a ideia na imagem será feito o "corte da imagem"

#Melhorias(se der):
# Aplicar para videos(WebCam)
# A Qualidade do corte pode melhorar com Realce de Contraste como pré-processamento
# Da pra fazer pós processamento


#Inicializar matriz binária com 1
def criarMascara(linhas, colunas):
    matriz = []
    for l in range(0, linhas):
        linha = []
        for c in range(0, colunas):
            linha.append(1)
        matriz.append(linha)
    return matriz


#Usado para "cortar as imagem"
def aplicarMascara(mascara, imagem, linhas, colunas):
    for l in range(0, linhas):
        for c in range(0, colunas):
            if mascara[l][c] == 0:
                imagem[l][c] = 0, 0, 0
    return imagem


#Mascara para cortar o fundo
def inverterMascara(mascara, linhas, colunas):
    for l in range(0, linhas):
        for c in range(0, colunas):
            if mascara[l][c] == 0:
                mascara[l][c] = 1
            else:
                mascara[l][c] = 0
    return mascara


#Juntando as imagens
def mesclar(foreground, background, linhas, colunas):
    for l in range(0, linhas):
        for c in range(0, colunas):
            r, g, b = foreground[l][c]
            if r == 0 and g == 0 and b == 0:
                foreground[l][c] = background[l][c]
    return foreground


def removerFundoVerde(foreground, background):
    #Convertendo para o sistema de cor HSV
    foreground_hsv = cv2.cvtColor(foreground, cv2.COLOR_BGR2HSV)

    #Parametros Iniciais
    linhas, colunas, canais = foreground_hsv.shape
    mascara = criarMascara(linhas, colunas)

    #Acessando o pixel verde e marcando a mascara
    for l in range(0,linhas):
        for c in range (0,colunas):
           hue, sat, val = foreground_hsv[l, c]
           if 38 <= hue <= 74 and sat >= 30 and val >= 30 : #HSV do OPENCV usa valores diferentes
               mascara[l][c] = 0

    #Cortando o foreground
    foreground_cortado = aplicarMascara(mascara, foreground_hsv, linhas, colunas)
    foreground_bgr = cv2.cvtColor(foreground_cortado, cv2.COLOR_HSV2BGR)

    #Cortando o background
    mascara_inv = inverterMascara(mascara, linhas, colunas)
    background_cortado = aplicarMascara(mascara_inv, background, linhas, colunas)

    #Concatenando os background e foreground
    img = mesclar(foreground_bgr, background_cortado, linhas, colunas)
    return img


def main():
    #Criando janelas
    JANELA_PRINCIPAL = "IMG ORIGINAL"
    JANELA_FUNDO = "IMG FUNDO"
    cv2.namedWindow(JANELA_PRINCIPAL, cv2.WINDOW_NORMAL)
    cv2.namedWindow(JANELA_FUNDO, cv2.WINDOW_NORMAL)

    #Abrindo imagens
    foreground = cv2.imread("recursos/teste1.png", cv2.IMREAD_COLOR)
    background = cv2.imread("recursos/konoha.jpg", cv2.IMREAD_COLOR)

    #Exibindo imagens
    cv2.imshow(JANELA_PRINCIPAL, foreground)
    cv2.imshow(JANELA_FUNDO, background)
    cv2.waitKey(0)

    img_cortada = removerFundoVerde(foreground, background)
    cv2.imshow("Imagem Cortada", img_cortada)
    cv2.waitKey(0)


main()
