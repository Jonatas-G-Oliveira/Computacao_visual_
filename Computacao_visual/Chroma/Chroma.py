import cv2

# -- CHROMA
# A imagem vai ser trabalhada em HSV -> color picker
# É necessario criar uma mascara binaria
#   - Onde for 1 será a imagem
#   - Onde for 0 será o verde
# Ao acessar o pixel da imagem verificamos se o mesmo esta no intervalo de matiz desejado
# e marcamos na mascara
# Aplicando a ideia na imagem será feito o "corte da imagem"

#Melhorias(se der):
# Colocar o fundo
# Aplicar para videos
# A Qualidade do corte pode melhorar com Realce de Contraste como pré-processamento
# Da pra fazer pós processamento


#Inicializar matriz com binária com 1
def criarMascara(linhas, colunas):
    matriz = []
    for l in range(0, linhas):
        linha = []
        for c in range(0, colunas):
            linha.append(1)
        matriz.append(linha)
    return matriz


#usar a mascara para "cortar a imagem" principal
def aplicarMascara(mascara, imagem, linhas, colunas):
    for l in range(0, linhas):
        for c in range(0, colunas):
            if mascara[l][c] == 0:
                imagem[l][c] = 0, 0, 0
    return imagem


#Usar a mascara para cortar o fundo
def inverterMascara(mascara, linhas, colunas):
    for l in range(0, linhas):
        for c in range(0, colunas):
            if mascara[l][c] == 0:
                mascara[l][c] = 1
            else:
                mascara[l][c] = 0
    return mascara


def removerFundoVerde(img):
    #Convertendo para Sistema de cor HSV
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    #Parametros Iniciais
    linhas, colunas, canais = img_hsv.shape
    mascara = criarMascara(linhas, colunas)

    #Acessando pixel
    for l in range(0,linhas):
        for c in range (0,colunas):
           hue, sat, val = img_hsv[l, c]
           if 38 <= hue <= 74 and sat >= 30 and val >= 30 : #HSV do OPENCV usa valores diferentes
               mascara[l][c] = 0

    img_cortada = aplicarMascara(mascara, img_hsv, linhas, colunas)
    img = cv2.cvtColor(img_cortada, cv2.COLOR_HSV2BGR)

    return img


def main():
    #Criando janela
    imagem_principal = "PlaceHolder"
    cv2.namedWindow(imagem_principal, cv2.WINDOW_AUTOSIZE)
    imagem_fundo = "Fundo"
    cv2.namedWindow(imagem_fundo, cv2.WINDOW_AUTOSIZE)


    #Abrindo e exibindo imagem
    img = cv2.imread("recursos/teste1.png", cv2.IMREAD_COLOR)
    fundo = cv2.imread("recursos/konoha.jpg", cv2.IMREAD_COLOR)
    fundo_redimensinado = cv2.resize(fundo,(500, 300))
    cv2.imshow(imagem_principal, img)
    cv2.imshow(imagem_fundo, fundo_redimensinado)
    cv2.waitKey(0)

    img_cortada = removerFundoVerde(img)
    cv2.imshow("Imagem Cortada", img_cortada)
    cv2.waitKey(0)
    #Desalocando memória
    #cv2.destroyWindow(nome_janela)

main()