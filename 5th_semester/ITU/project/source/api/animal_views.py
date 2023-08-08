from rest_framework.decorators import api_view, permission_classes
from rest_framework.response import Response
from rest_framework.permissions import IsAuthenticated

from .models import AnimalModel
from .serializers import AnimalSerializer
from .utils import hasRole

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def createAnimal(request):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')
	serializer = AnimalSerializer(data=request.data)
	if serializer.is_valid():
		serializer.save()
		response = 'Animal is created.'
	else:
		response = 'Animal is not created.'
	return Response(response)

@api_view(['POST', 'GET'])
@permission_classes([IsAuthenticated])
def detailAnimal(request, pk):
	if request.method == 'GET':
		user = AnimalModel.objects.get(id=pk)
		serializer = AnimalSerializer(user, many=False)
		return Response(serializer.data)

	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')
	animal = AnimalModel.objects.get(id=pk)
	serializer = AnimalSerializer(instance=animal, data=request.data, many=False)
	if serializer.is_valid():
		serializer.save()
		response = 'Animal is updated.'
	else:
		response = 'Animal is not updated.'
	return Response(response)

@api_view(['GET'])
def listAnimal(request):
	animals = AnimalModel.objects.all()
	serializer = AnimalSerializer(animals, many=True)
	return Response(serializer.data)

@api_view(['DELETE'])
@permission_classes([IsAuthenticated])
def deleteAnimal(request, pk):
	if not hasRole(request, 'Caretaker'):
		return Response('You do not have permission.')
	try:
		animal = AnimalModel.objects.get(id=pk)
		animal.delete()
		response = 'Animal is deleted.'
	except:
		response = 'Animal is not deleted.'
	return Response(response)