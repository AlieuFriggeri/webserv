/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequestHandler.class.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:44:20 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/02 10:48:42 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_REQUEST_HANDLER_CLASS_HPP
# define DELETE_REQUEST_HANDLER_CLASS_HPP

#include "RequestHandler.class.hpp"

class DeleteRequestHandler: public RequestHandler
{
	private:

	public:
		DeleteRequestHandler(void);
		~DeleteRequestHandler(void);

		HttpRespond	handleRequest(HttpRequest req);		
};

#endif