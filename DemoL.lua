free_memory = function(size)
	for i = 0, size, 1 do 
		kernel.delToList()
	end
end

function main() --Эта функция будет вызвана один раз при запуске приложения
	for	i = 0, 9, 1 do
		kernel.addToList()
		kernel.getElem(i):setValue(math.random(1, 20))
		kernel.getElem(i):printValue()
	end
	
	free_memory(9)
end